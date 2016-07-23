#!/usr/bin/env python
import control
import matplotlib.pyplot as plot
import numpy
import random
import sys


# matlab functions
def stepinfo(t, y):
    t_r = t[next(i for i in range(len(y) - 1)
                 if y[i] > y[-1] * .9)] - t[0]
    try:
        t_s = t[next(len(y) - i for i in range(2, len(y) - 1)
                     if abs(y[-i] / y[-1]) > 1.02)] - t[0]
    except StopIteration:
        t_s = sys.maxsize
    os = y.max() / y[-1] - 1

    return t_r, t_s, os

def stepresponse(Kp, Ti, Td):
    G = Kp * control.tf([Ti * Td, Ti, 1], [Ti, 0])
    F = control.tf(1, [1, 6, 11, 6, 0])

    system = control.feedback(control.series(G, F), 1)
    t = numpy.arange(0, 100, 0.01)
    y, T = control.step(system, t)
    t_r, t_s, M_p = stepinfo(T, y)

    ISE = sum(numpy.power(numpy.subtract(y, 1), 2))

    return ISE, t_r, t_s, M_p


# utility
def weighted_choice(choices, total):
    r = random.uniform(0, total)
    upto = 0
    for k, v in choices.items():
        if upto + v >= r:
            return k
        upto += v
    assert False, "Can't get here"


# algorithm
def fitness(Kp, Ti, Td):
    ISE, t_r, t_s, M_p = stepresponse(Kp, Ti, Td)

    return 1. / float(ISE + t_r + t_s + M_p)

def genetic(population, crossover, mutation, iterations):
    print('Running GA with p={}, itt={}, c%={}, m%={}'.format(population,
                                                              iterations,
                                                              crossover,
                                                              mutation))

    range_Kp = [x / 100. for x in range(200, 1800)]
    range_Ti = [x / 100. for x in range(105, 942)]
    range_Td = [x / 100. for x in range(26, 237)]

    pop_Kp = random.sample(range_Kp, population)
    pop_Ti = random.sample(range_Ti, population)
    pop_Td = random.sample(range_Td, population)

    best_matches = []

    current = {k: 0 for k in zip(pop_Kp, pop_Ti, pop_Td)}
    for Kp, Ti, Td in current.keys():
        current[(Kp, Ti, Td)] = fitness(Kp, Ti, Td)
    elite = sorted(current, key=current.get, reverse=True)
    cost = sum(current.values())

    for itt in range(iterations):
        print('> iteration {} of {}... '.format(itt + 1, iterations), end='',
              flush=True)
        children = []

        # parent-selection
        while len(children) < population:
            parent = weighted_choice(current, cost)

            # crossover
            if random.random() < crossover:
                mate = weighted_choice(current, cost)

                child0 = []
                child1 = []
                for (gene0, gene1) in zip(parent, mate):
                    if random.random() < 0.5:
                        child0.append(gene0)
                        child1.append(gene1)
                    else:
                        child0.append(gene1)
                        child1.append(gene0)
                child0 = tuple(child0)  # pylint: disable=R0204
                child1 = tuple(child1)  # pylint: disable=R0204

                children.append(child0)
                children.append(child1)
            else:
                children.append(parent)

        # mutation
        current = {}
        for c in children:
            if random.random() < mutation:
                idx = random.random()
                if idx < 0.33:
                    c = tuple([c[0], c[1], random.choice(range_Td)])
                elif idx < 0.66:
                    c = tuple([c[0], random.choice(range_Ti), c[2]])
                elif idx < 0.99:
                    c = tuple([random.choice(range_Kp), c[1], c[2]])

            current[c] = fitness(c[0], c[1], c[2])

        # elite
        for item in elite[:2]:
            current[item] = fitness(item[0], item[1], item[2])

        elite = sorted(current, key=current.get, reverse=True)
        cost = sum(current.values())

        best_matches.append(1 - current[elite[0]])
        print('done!')

    # graph
    plot.clf()
    plot.plot(range(iterations), best_matches)

    plot.xlabel('iteration')
    plot.ylabel('best fitness')
    plot.title('Genetic Algorithm')
    plot.grid(True)
    plot.savefig('q1-i{}-p{}-c{}-m{}.png'.format(iterations, population,
                                                 crossover, mutation))
    plot.show()


def main():
    for population in (10, 15, 50):
        for iterations in (10, 15, 150):
            genetic(population=population, crossover=0.6, mutation=0.25,
                    iterations=iterations)

    genetic(population=50, crossover=0.6, mutation=0.05, iterations=50)
    genetic(population=50, crossover=0.6, mutation=0.30, iterations=50)

    genetic(population=50, crossover=0.7, mutation=0.25, iterations=50)
    genetic(population=50, crossover=0.8, mutation=0.25, iterations=50)


if __name__ == '__main__':
    main()
