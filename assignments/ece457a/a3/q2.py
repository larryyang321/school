#!/usr/bin/env python
import matplotlib.pyplot as plot
import random
import sys


cities = [
    (1150.0, 1760.0),
    (630.0, 1660.0),
    (40.0, 2090.0),
    (750.0, 1100.0),
    (750.0, 2030.0),
    (1030.0, 2070.0),
    (1650.0, 650.0),
    (1490.0, 1630.0),
    (790.0, 2260.0),
    (710.0, 1310.0),
    (840.0, 550.0),
    (1170.0, 2300.0),
    (970.0, 1340.0),
    (510.0, 700.0),
    (750.0, 900.0),
    (1280.0, 1200.0),
    (230.0, 590.0),
    (460.0, 860.0),
    (1040.0, 950.0),
    (590.0, 1390.0),
    (830.0, 1770.0),
    (490.0, 500.0),
    (1840.0, 1240.0),
    (1260.0, 1500.0),
    (1280.0, 790.0),
    (490.0, 2130.0),
    (1460.0, 1420.0),
    (1260.0, 1910.0),
    (360.0, 1980.0)
]

distances = [[0 for _ in enumerate(cities)] for _ in enumerate(cities)]
for i, src in enumerate(cities):
    for j, dest in enumerate(cities):
        dist = ((src[0] - dest[0]) ** 2 + (src[1] - dest[1]) ** 2) ** (1/2.)
        distances[i][j] = dist
        distances[j][i] = dist

pheromones = list()


def weighted_choice(choices, total):
    r = random.uniform(0, total)
    upto = 0
    for k, v in choices:
        if upto + v >= r:
            return k
        upto += v
    assert False, "Can't get here"


class Ant:
    ALPHA = 1.0
    BETA = 5.0
    PHER = 1.0 / len(cities)
    Q = 100.0
    RHO = 0.5

    def __init__(self, city_current):
        self.path = [city_current]
        self.distance = 0

    def pheromones_to_city(self, city):
        return pheromones[self.path[-1]][city] ** self.ALPHA * \
               distances[self.path[-1]][city] ** (-self.BETA)

    def pheromone_drop(self):
        src = self.path[-2]
        dest = self.path[-1]
        pheromones[src][dest] += (self.Q / self.distance)
        pheromones[dest][src] = pheromones[src][dest]

    @staticmethod
    def pheromone_evaporate():
        for i, _ in enumerate(cities):
            for j, _ in enumerate(cities):
                pheromones[i][j] *= (1. - Ant.RHO)
                if pheromones[i][j] < 0:
                    pheromones[i][j] = Ant.PHER

    @staticmethod
    def pheromone_reset():
        global pheromones  # pylint: disable=W0603
        pheromones = [[Ant.PHER for _ in range(len(cities))]
                      for _ in range(len(cities))]

    def next_city(self):
        destinations = list(set(range(len(cities))) - set(self.path))
        weights = [self.pheromones_to_city(dest) for dest in destinations]

        city = weighted_choice(zip(destinations, weights), sum(weights))

        self.distance += distances[self.path[-1]][city]
        self.path.append(city)

        self.pheromone_drop()

        if len(self.path) == len(cities):
            self.distance += distances[self.path[-1]][self.path[0]]


def aco(num_ants=len(cities), online_update=True, q=100.0, rho=0.5,
        iterations=100):
    Ant.Q = q
    Ant.RHO = rho

    Ant.pheromone_reset()

    best_path = None
    current = sys.maxsize

    for _ in range(iterations):
        ants = [Ant(i % len(cities)) for i in range(num_ants)]

        for _ in range(len(cities) - 1):
            for ant in ants:
                ant.next_city()

        if online_update:
            Ant.pheromone_evaporate()

        for ant in ants:
            if ant.distance < current:
                best_path = ant.path
                current = ant.distance

    # graph
    x, y = zip(*[cities[x] for x in best_path + best_path[:1]])

    plot.clf()
    plot.plot(x, y, marker='x', linestyle='--')

    plot.title('Ant Colony Optimization')
    plot.grid(True)
    plot.savefig('figs/q2/n{}-q{}-r{}{}.png'.format(
        num_ants, Ant.Q, Ant.RHO, '-online' if online_update else ''))
    plot.show()


def main():
    for q in (75.0, 100.0, 125.0):
        for rho in (0.4, 0.5, 0.6):
            for num_ants in (len(cities) - 5, len(cities), len(cities) + 5):
                for online_update in (False, True):
                    aco(num_ants=num_ants, online_update=online_update, q=q,
                        rho=rho)


if __name__ == '__main__':
    main()
