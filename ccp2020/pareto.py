import sys

def main():
    lines = list(sys.stdin.readlines())[1:]
    splitted = map(str.split, lines)
    points = [(int(x[0]), int(x[1])) for x in splitted]

    points = sorted(points, key=lambda x: x[0])

    pareto = 0
    while points:
        pareto += 1
        current = points[-1]
        survived = []
        for p in points:
            if p[1] >= current[1] and p != current:
                survived.append(p)
        points = survived
    print(pareto)


if __name__ == '__main__':
    main()



