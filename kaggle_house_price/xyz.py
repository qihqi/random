#%%
import sys
import gam
import json
import numpy as np
import xgboost as xgb
import pandas as pd
from sklearn.model_selection import train_test_split
import os
os.environ['CUDA_VISIBLE_DEVICES'] = '0'

categorical_features = [
    'MSSubClass',
    'MSZoning',
    'Street',
    'Alley',
    'LotShape',
    'LandContour',
    'Utilities',
    'LotConfig',
    'LandSlope',
    'Neighborhood',
    'Condition1',
    'Condition2',
    'BldgType',
    'HouseStyle',
    'RoofStyle',
    'RoofMatl',
    'Exterior1st',
    'Exterior2nd',
    'MasVnrType',
    'ExterQual',
    'ExterCond',
    'Foundation',
    'BsmtQual',
    'BsmtCond',
    'BsmtExposure',
    'BsmtFinType1',
    'BsmtFinType2',
    'Heating',
    'HeatingQC',
    'CentralAir',
    'Electrical',
    'KitchenQual',
    'Functional',
    'FireplaceQu',
    'GarageType',
    'GarageFinish',
    'GarageQual',
    'GarageCond',
    'PavedDrive',
    'PoolQC',
    'Fence',
    'SaleType',
    'SaleCondition',
]

excl_feature = [
    'MiscFeature',
    'MiscVal',
]

label = 'SalePrice'

from sklearn.preprocessing import OneHotEncoder
import itertools

def make_data_df(df, df_test):
    numerical = sorted(set(df.columns) - set(
        categorical_features + excl_feature  + [label]))
    encoder = OneHotEncoder()
    encoder.fit(pd.concat([df, df_test])[categorical_features])
    encoded = encoder.transform(df[categorical_features])
    feat_names_onehot = []
    allowed_interactions = []
    starting_id = len(numerical)
    for i in range(starting_id):
        allowed_interactions.append((i,))
    for name, cat_value in zip(categorical_features, encoder.categories_):
        bucket = []
        for v in cat_value:
            fname = '{}={}'.format(name, v)
            feat_names_onehot.append(fname)
            bucket.append(fname)
            starting_id += 1
        allowed_interactions.append(tuple(bucket))

    df_onehot = pd.DataFrame(
        encoded.toarray(), columns=feat_names_onehot, dtype=float)
    df_onehot.index = df.index
    
    df_test_onehot = pd.DataFrame(
        encoder.transform(df_test[categorical_features]).toarray(),
        columns=feat_names_onehot, dtype=float)
    df_test_onehot.index = df_test.index

    df_final = pd.concat([df[numerical], df_onehot], axis=1)
    df_final_test = pd.concat([df_test[numerical], df_test_onehot], axis=1)

    return df_final, df_final_test, allowed_interactions

# %%

df = pd.read_csv('train.csv', index_col='Id')
df_test = pd.read_csv('test.csv', index_col='Id')
df_final, df_test_final, allowed_interactions = make_data_df(df, df_test)

df_train, df_validation, train_label, val_label = train_test_split(df_final, df[label])

# %%
# specify parameters via map
train = xgb.DMatrix(df_train, label=np.log(train_label))
val = xgb.DMatrix(df_validation, label=np.log(val_label))
param = {'max_depth': 7, 'eta': 0.01, 'objective': 'reg:squarederror'}
# param['nthread'] = 4
param['eval_metric'] = 'rmse'
param['tree_method'] = 'hist'
param['subsample'] = 0.8 
param['interaction_constraints'] = [] #allowed_interactions
num_round = 2000

# %%
bst = xgb.train(
    param, train, num_round, [(train, 'train'), (val, 'val')],
    early_stopping_rounds=10)

print('best round {}'.format(bst.best_iteration))

# %%
res = bst.predict(xgb.DMatrix(df_test_final), 
                  iteration_range=(0, bst.best_iteration))

with open('submission.csv', 'w') as f:
    print('Id,SalePrice', file=f)
    for x, y in zip(df_test_final.index, np.exp(res)):
        print('{},{}'.format(x, y), file=f)
        
bst.save_model('saved_model.json')

# %%
splitted = gam.build_gam_from_xgboost('saved_model.json')
for k, s in splitted.items():
    try:
        r = s.predict(val)
    except:
        print(k)
        sys.exit(1)

res = [np.array(s.predict(val)) for s in splitted.values()]
res = np.sum(res)
res1 = np.array(bst.predict(val))

print('diff {}', (res1-res).norm())
    