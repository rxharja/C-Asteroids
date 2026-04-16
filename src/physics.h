//
// Created by redonxharja on 4/15/26.
//

#ifndef ASTEROIDS_PHYSICS_H
#define ASTEROIDS_PHYSICS_H
#include "vector.h"

typedef struct Body {
    Polygon shape;
    Vector2 velocity;
} Body;

Body create_body(Polygon p, Vector2 velocity);

void move_body(Body *body);

void destroy_body(const Body *body);

#endif //ASTEROIDS_PHYSICS_H
