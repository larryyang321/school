#!/usr/bin/env python
from functools import partial
from queue import Queue

def get_maze():
    return [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
            [1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
            [1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0],
            [0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0],
            [0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
            [0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1],
            [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0],
            [0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0],
            [1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0]]

S = (11, 2)
E1 = (19, 23)
E2 = (21, 2)

BOTTOM_LEFT = (0, 0)
TOP_RIGHT = (24, 24)

def moves(maze, position):
    # up, right, down, left
    possible = [(position[0], position[1] + 1),
                (position[0] + 1, position[1]),
                (position[0], position[1] - 1),
                (position[0] - 1, position[1])]

    actual = list()
    length = len(maze)
    for x, y in possible:
        if x < 0 or x >= length or y < 0 or y >= length:
            continue
        if maze[x][y]:
            continue
        actual.append((x, y))
    return actual


def astar(maze, start, end):
    cost = lambda distance, current: distance + ((end[0] - current[0]) ** 2 + (end[1] - current[1]) ** 2) ** 0.5

    visited = set()
    will_visit = set()

    queue = Queue()
    queue.put((start, list()))
    will_visit.add(start)

    while not queue.empty():
        current, path = queue.get()
        path.append(current)
        visited.add(current)

        if current == end:
            return path, len(path), len(visited)

        options = moves(maze, current)
        options.sort(key=partial(cost, len(path)))
        for potential in options:
            if potential in will_visit:
                continue

            will_visit.add(potential)
            queue.put((potential, path[:]))

def bfs(maze, start, end):
    visited = set()
    will_visit = set()

    queue = Queue()
    queue.put((start, list()))
    will_visit.add(start)

    while not queue.empty():
        current, path = queue.get()
        path.append(current)
        visited.add(current)

        if current == end:
            return path, len(path), len(visited)

        for potential in moves(maze, current):
            if potential in will_visit:
                continue

            will_visit.add(potential)
            queue.put((potential, path[:]))


def dfs(maze, start, end):
    visited = set()
    will_visit = set()

    stack = list()
    stack.append((start, list()))
    will_visit.add(start)

    while stack:
        current, path = stack.pop()
        path.append(current)
        visited.add(current)

        if current == end:
            return path, len(path), len(visited)

        for potential in moves(maze, current):
            if potential in will_visit:
                continue

            will_visit.add(potential)
            stack.append((potential, path[:]))


def main():
    maze = get_maze()

    for fx, name in ((bfs, 'Breadth-First Search'),
                     (dfs, 'Depth-First Search'),
                     (astar, 'A-Star Search')):
        for start, end in ((S, E1), (S, E2), (BOTTOM_LEFT, TOP_RIGHT)):
            print('{}: start = {}, end = {}'.format(name, start, end))
            print(fx(maze, start, end))
        print()


if __name__ == '__main__':
    main()
