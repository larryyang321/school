#!/usr/bin/env python
import control
import numpy
import random


def stepinfo(t, y):
    t_r = t[next(i for i in range(len(y) - 1)
                 if y[i] > y[-1] * .9)] - t[0]
    t_s = t[next(len(y) - i for i in range(2, len(y) - 1)
                 if abs(y[-i] / y[-1]) > 1.02)] - t[0]
    os = y.max() / y[-1] - 1

    return t_r, t_s, os

def stepresponse(Kp, Ti, Td):
    G = Kp * control.tf([Ti * Td, Ti, 1], [Ti, 0])
    F = control.tf(1, [1, 6, 11, 6, 0])

    sys = control.feedback(control.series(G, F), 1)
    t = numpy.arange(0, 100, 0.01)
    y, T = control.step(sys, t)
    t_r, t_s, M_p = stepinfo(T, y)

    ISE = sum(numpy.power(numpy.subtract(y, 1), 2))

    return ISE, t_r, t_s, M_p


def fitness(Kp, Ti, Td):
    ISE, t_r, t_s, M_p = stepresponse(Kp, Ti, Td)

    return float(ISE + t_r + t_s + M_p)

def genetic(population, iterations):
    range_Kp = [x / 100. for x in range(200, 1800)]
    range_Ti = [x / 100. for x in range(105, 942)]
    range_Td = [x / 100. for x in range(26, 237)]

    pop_Kp = random.sample(range_Kp, population)
    pop_Ti = random.sample(range_Ti, population)
    pop_Td = random.sample(range_Td, population)

    current = {k: 0 for k in zip(pop_Kp, pop_Ti, pop_Td)}
    for Kp, Ti, Td in current.keys():
        current[(Kp, Ti, Td)] = fitness(Kp, Ti, Td)

    cost = sum(v for v in current.values()) / len(current)

    for _ in range(iterations):
        best = sorted(current, key=current.get, reverse=True)[:2]
        print(best)

        return

    #         select best-ranking individuals to reproduce
    #         mate pairs at random
    #         apply crossover operator
    #         apply mutation operator
    #         evaluate each individual's fitness
    #         determine population's average fitness
    # until terminating condition (e.g. until at least one individual has
    #          the desired fitness or enough generations have passed)

def main():
    genetic(population=50, iterations=150)


if __name__ == '__main__':
    main()
