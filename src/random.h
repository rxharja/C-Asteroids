#ifndef ASTEROIDS_RANDOM_H
#define ASTEROIDS_RANDOM_H

#include "vector.h"

double random_float(void);

int random_range(int min, int max);

int random_bool(void);

int random_int_choice(int one, int two);

// point on a circle of radius between 0.7*r and r at the given angle (radians)
Vector2 random_vector(double r, double radian);

Vector2 random_edge_position(void);

#endif // ASTEROIDS_RANDOM_H
