# %%
import uuid
from collections import defaultdict
import json
import copy

import xgboost as xgb


def build_gam_from_xgboost(path):
    bst = xgb.Booster()
    bst.load_model(path)
    
    trees = bst.get_dump(dump_format='json')
    feat_name_to_tree_id = defaultdict(list)
    
    for i, t in enumerate(trees):
        tree = json.loads(t)
        if 'split' in tree:
            feat_name_to_tree_id[tree['split']].append(i)
        else:
            feat_name_to_tree_id['CONSTANT'].append(i)
            
        
    with open(path) as f:
        raw_model = json.loads(f.read()) 
        
    trees = raw_model['learner']['gradient_booster']['model']['trees']
    raw_model['learner']['gradient_booster']['model']['trees'] = []
    
    fname_to_model = {}
    for fname, ids in feat_name_to_tree_id.items():
        new_model = copy.deepcopy(raw_model)
        new_trees = []
        for new_id, i in enumerate(ids):
            trees[i]['id'] = new_id
            new_trees.append(trees[i])
        new_model['learner']['gradient_booster']['model']['trees'] = new_trees 
        new_model['learner']['gradient_booster']['model']['gbtree_model_param']['num_trees'] = str(len(new_trees) - 1)
        new_model['learner']['gradient_booster']['model']['tree_info'] = [0] * (len(new_trees) - 1)
        new_model['learner']['attributes']['best_ntree_limit'] = str(len(new_trees) - 1)
        new_model['learner']['attributes']['best_iteration'] = str(len(new_trees) - 1)
        
        new_model_json = json.dumps(new_model)

        filename = '/tmp/temp{}.json'.format(uuid.uuid4().hex)
        with open(filename, 'w') as f:
            f.write(new_model_json)
            f.close() 
        model = xgb.Booster({'nthread':1})
        try:
            model.load_model(filename)
        except Exception as e:
            print(len(new_trees))
            raise
        fname_to_model[fname] = model
    return fname_to_model