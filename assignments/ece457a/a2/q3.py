#!/usr/bin/env python
import copy
import csv
import functools
import random


def parse_distances():
    distances = []
    with open('q3-distance.csv', 'r') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            distances.append([int(x) for x in row])
    return distances

def parse_flows():
    flows = []
    with open('q3-flow.csv', 'r') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            flows.append([int(x) for x in row])
    return flows


def get_cost(distances, flows, solution):
    cost = 0
    for i, from_list in enumerate(solution):
        for j, from_item in enumerate(from_list):
            for k, to_list in enumerate(solution):
                for l, to_item in enumerate(to_list):
                    cost += (distances[i * 5 + j][k * 5 + l]
                             * flows[from_item - 1][to_item - 1])

    return cost


def get_neighbours(solution, tabu_list, distances, flows, best_cost):
    sols = []
    for i, _ in enumerate(solution):
        for j, _ in enumerate(solution[i]):
            for k, _ in enumerate(solution):
                for l, _ in enumerate(solution[k]):
                    if i == k and j == l:
                        continue

                    new_solution = copy.deepcopy(solution[:])

                    temp = new_solution[i][j]
                    new_solution[i][j] = new_solution[k][l]
                    new_solution[k][l] = temp

                    if tabu_list[i][j] or tabu_list[k][l]:
                        if best_cost < get_cost(distances, flows,
                                                new_solution):
                            continue

                    sols.append(copy.deepcopy(new_solution[:]))

    return sols

def get_swap(solution0, solution1):
    swapped = []
    for i, row in enumerate(solution0):
        for j, item in enumerate(row):
            if solution1[i][j] != item:
                swapped.append((i, j))

    return swapped


def ts(distances, flows, ttl, iterations):
    tabu_list = [[0] * 5 for i in range(4)]

    solution = [[1, 2, 3, 4, 5],
                [6, 7, 8, 9, 10],
                [11, 12, 13, 14, 15],
                [16, 17, 18, 19, 20]]
    cost = get_cost(distances, flows, solution)
    best_cost = cost

    for _ in range(iterations):
        neighbours = get_neighbours(solution, tabu_list, distances, flows,
                                    best_cost)
        neighbours = random.sample(neighbours, min(20, len(neighbours)))
        neighbours.sort(key=functools.partial(get_cost, distances, flows))
        try:
            best_neighbour = neighbours[0]
        except IndexError:
            break

        neighbour_cost = get_cost(distances, flows, best_neighbour)

        if neighbour_cost < best_cost:
            best_cost = neighbour_cost
            ttl = 1
        elif neighbour_cost < cost:
            ttl = ttl - 1 if ttl > 1 else ttl
        elif neighbour_cost > cost:
            ttl += 1

        for i, row in enumerate(tabu_list):
            for j, item in enumerate(row):
                if item:
                    tabu_list[i][j] = item - 1

        for swap in get_swap(solution, best_neighbour):
            tabu_list[swap[0]][swap[1]] = ttl

        solution = best_neighbour
        cost = neighbour_cost

    return solution, cost


def main():
    distances = parse_distances()
    flows = parse_flows()
    iterations = 250
    ttl = 5

    solution, cost = ts(distances, flows, ttl, iterations)
    print(solution, cost)


if __name__ == '__main__':
    main()
