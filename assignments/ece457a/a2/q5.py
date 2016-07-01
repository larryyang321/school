#!/usr/bin/env python
import math
import pprint
import random

sets = [
    (32, 5),
    (33, 5),
    (33, 6),
    (34, 5),
    (36, 5),
    (37, 5),
    (37, 6),
    (38, 5),
    (39, 5),
    (39, 6),
    (44, 6),
    (45, 6),
    # (45, 7),
    (46, 7),
    (48, 7),
    (53, 7),
    (54, 7),
    (55, 9),
    (61, 9),
    (65, 9),
]

def parse_problem_set(c, m):
    costs = {}
    nodes = {}

    in_cost = False
    in_position = False
    with open('datasets/A-n{}-k{}.vrp'.format(c, m), 'r') as f:
        for line in f.readlines():
            if line.startswith('NODE_COORD_SECTION'):
                in_position = True
            elif line.startswith('DEMAND_SECTION'):
                in_cost = True
            elif line.startswith('DEPOT_SECTION'):
                break
            elif in_cost:
                data = line.split(' ')
                costs[int(data[0])] = int(data[1])
            elif in_position:
                data = line.split(' ')
                nodes[int(data[1])] = (int(data[2]), int(data[3].strip()))

    return costs, nodes

def parse_answer_set(c, m):
    paths = []

    with open('datasets/opt-A-n{}-k{}'.format(c, m), 'r') as f:
        for line in f.readlines():
            if line.startswith('Route'):
                data = [int(x) for x in line.split(': ')[1].split(' ')[:-1]]
                paths.append(data)
            else:
                data = line.split(' ')[1]
                return paths, float(data)


class Annealer:
    def __init__(self, temperature, final_temperature, alpha, iterations):
        self.temperature = temperature
        self.final_temperature = final_temperature
        self.alpha = alpha
        self.iterations = iterations

        self.paths = []
        self.current_cost = 0
        self.best_cost = 0

    @staticmethod
    def get_cost(paths, nodes, costs):
        routes = [[1] + path + [1] for path in paths]

        c = 0
        for i, path in enumerate(paths):
            for city in path:
                c += costs[city]
            for back, path in zip(routes[i][:-1], routes[i][1:]):
                xd = nodes[back][0] - nodes[path][0]
                yd = nodes[back][1] - nodes[path][1]
                c += ((xd ** 2 + yd ** 2)) ** 0.5

        return c

    def get_neighbour(self):
        routes = range(len(self.paths))
        from_path, to_path = random.choice(routes), random.choice(routes)
        if not self.paths[from_path]:
            return self.get_neighbour()

        from_index = random.choice(range(len(self.paths[from_path])))
        to_index = random.choice(list(range(len(self.paths[to_path]))) or [0])

        if from_path == to_path and from_index == to_index:
            return self.get_neighbour()

        paths = self.paths[:]

        item = paths[from_path].pop(from_index)
        paths[to_path].insert(to_index, item)
        return paths

    def next_temperature(self):
        self.temperature *= self.alpha
        if self.temperature <= self.final_temperature:
            return None

        return self.temperature

    def run(self, m, nodes, costs):
        self.paths = [[()] * 0 for i in range(m)]
        for i, node in enumerate(nodes.keys()):
            if node == 1:
                continue

            self.paths[i % m].append(node)

        self.current_cost = self.get_cost(self.paths, nodes, costs)
        self.best_cost = self.current_cost

        for _ in range(self.iterations):
            temp = self.next_temperature()
            if not temp:
                break

            neighbour = self.get_neighbour()
            cost = self.get_cost(neighbour, nodes, costs)

            if cost < self.best_cost:
                self.paths = neighbour
                self.current_cost = cost
                self.best_cost = cost
            elif random.random() > math.exp(-(abs(self.current_cost - cost)
                                              / self.temperature)):
                self.paths = neighbour
                self.current_cost = cost

        return self.paths, round(self.current_cost, 2)


def main():
    for c, m in sets:
        costs, nodes = parse_problem_set(c, m)

        print()
        print('>> SET: n={}, k={}'.format(c, m))

        print('> T={}->{}, alpha={}, iterations={}'.format(
            1000, 1, 0.85, 100))
        paths, cost = Annealer(1000, 1, 0.85, 100).run(m, nodes, costs)
        print('Final cost:', cost)
        pprint.pprint(paths)

        print('> T={}->{}, alpha={}, iterations={}'.format(
            600, 100, 0.96, 100))
        paths, cost = Annealer(600, 100, 0.96, 100).run(m, nodes, costs)
        print('Final cost:', cost)
        pprint.pprint(paths)

        print('> Optimal Answers')
        paths, cost = parse_answer_set(c, m)
        print('Final cost:', cost)
        pprint.pprint(paths)


if __name__ == '__main__':
    main()
