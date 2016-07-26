#!/usr/bin/env python
import random
import sys

from matplotlib.ticker import LinearLocator, FormatStrFormatter
from mpl_toolkits.mplot3d import Axes3D  # pylint: disable=W0611
import matplotlib.pyplot as plot


# vector math
class Infix:
    def __init__(self, function):
        self.function = function
    def __ror__(self, other):
        return Infix(lambda x, self=self, other=other: self.function(other, x))
    def __or__(self, other):
        return self.function(other)

add = Infix(lambda xs, ys:
            tuple(x + y for x, y in zip(
                xs if isinstance(xs, tuple) else tuple([xs] * len(ys)),
                ys if isinstance(ys, tuple) else tuple([ys] * len(xs)))))
sub = Infix(lambda xs, ys:
            tuple(x - y for x, y in zip(
                xs if isinstance(xs, tuple) else tuple([xs] * len(ys)),
                ys if isinstance(ys, tuple) else tuple([ys] * len(xs)))))
mul = Infix(lambda xs, ys:
            tuple(x * y for x, y in zip(
                xs if isinstance(xs, tuple) else tuple([xs] * len(ys)),
                ys if isinstance(ys, tuple) else tuple([ys] * len(xs)))))


# algorithm
class Particle:
    COGNITIVE = 2.0
    SOCIAL = 2.5
    ZETA = COGNITIVE + SOCIAL
    # assert ZETA > 4.

    INERTIA = 1.0
    # assert 0.8 <= INERTIA <= 1.2

    CONSTRICTION = 2. / abs(2 - ZETA - (ZETA ** 2 - 4 * ZETA) ** (1/2.))

    def __init__(self, min_x=-5, max_x=5, min_y=-5, max_y=5, min_velocity=-0.5,
                 max_velocity=0.5, vmax=False):
        self.min_x = min_x
        self.max_x = max_x
        self.min_y = min_y
        self.max_y = max_x

        self.v_max = max_velocity if vmax else sys.maxsize

        self.position = (random.uniform(min_x, max_x),
                         random.uniform(min_y, max_y))
        self.velocity = (random.uniform(min_velocity, max_velocity),
                         random.uniform(min_velocity, max_velocity))

    @staticmethod
    def camelback(x, y):
        term0 = (4 - 2.1 * x ** 2 + (x ** 4) / 3.) * x ** 2
        term1 = x * y
        term2 = (-4 + 4 * y ** 2) * y ** 2
        return term0 + term1 + term2

    def fitness(self):
        x, y = self.position
        return self.camelback(x, y)

    def update_position(self):
        self.position = self.position |add| self.velocity

        x, y = self.position
        if x < self.min_x:
            self.position = (self.min_x, self.position[1])
        elif x > self.max_x:
            self.position = (self.max_x, self.position[1])

        if y < self.min_y:
            self.position = (self.position[0], self.min_y)
        elif y > self.max_y:
            self.position = (self.position[0], self.max_y)

    def update_velocity(self, gbest, pbest):
        inertia = Particle.INERTIA |mul| self.velocity

        cognitive = Particle.COGNITIVE * random.random()
        cognitive = cognitive |mul| (pbest |sub| self.position)

        social = Particle.SOCIAL * random.random()
        social = social |mul| (gbest |sub| self.position)

        velocity = inertia |add| cognitive |add| social
        self.velocity = Particle.CONSTRICTION |mul| velocity

        x, y = self.velocity
        if x > self.v_max:
            self.velocity = (self.v_max, self.velocity[1])
        if y > self.v_max:
            self.velocity = (self.velocity[0], self.v_max)


def pso(num_particles=50, constriction=False, inertia_weight=False, vmax=False,
        global_best=False, iterations=50):
    if not constriction:
        Particle.CONSTRICTION = 1.0

    particles = [Particle(vmax=vmax) for _ in range(num_particles)]

    gbest = None
    nbest = None
    pbest = sys.maxsize

    for itt in range(iterations):
        if inertia_weight:
            Particle.INERTIA = (itt / iterations) * 2.0

        best = sys.maxsize
        for particle in particles:
            fitness = particle.fitness()
            if fitness < best:
                nbest = particle.position
                best = fitness
        if best < pbest:
            pbest = best
            gbest = nbest

        for particle in particles:
            particle.update_velocity(gbest if global_best else nbest, pbest)
            particle.update_position()

    # graph
    x, y = zip(*[p.position for p in particles])
    z = [p.fitness() for p in particles]

    fig = plot.figure()
    axis = fig.gca(projection='3d')
    axis.set_xlim(-5, 5)
    axis.set_ylim(-5, 5)
    axis.set_zlim(min(z), max(z))
    axis.zaxis.set_major_locator(LinearLocator(10))
    axis.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

    axis.scatter(x, y, z)

    plot.xlabel('x')
    plot.ylabel('y')
    plot.title('Particle Swarm Optimization\n'
               'Best Fitness = ({}, {}) -> {}'.format(round(gbest[0], 2),
                                                      round(gbest[1], 2),
                                                      round(pbest, 4)))
    plot.savefig('figs/q3/n{}{}{}{}{}.png'.format(
        num_particles, '-inrta' if inertia_weight else '',
        '-cnstr' if constriction else '', '-vmax' if vmax else '',
        '-global' if global_best else '-neighbourhood'))
    plot.show()


def main():
    for global_best in (False, True):
        pso(global_best=global_best)
        pso(global_best=global_best, constriction=True)
        pso(global_best=global_best, inertia_weight=True)
        pso(global_best=global_best, vmax=True)


if __name__ == '__main__':
    main()
