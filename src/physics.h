//
// Created by redonxharja on 4/15/26.
//

#ifndef ASTEROIDS_PHYSICS_H
#define ASTEROIDS_PHYSICS_H
#include "polygon.h"
#include "vector.h"

typedef struct Body {
    Polygon shape;
    Vector2 position; // centroid
    Vector2 acceleration;
    Vector2 velocity;
    double angle;
    double angular_velocity;
    double drag;
    double mass;
} Body;

Body create_body(Polygon shape, Vector2 position, Vector2 velocity, double angle, double angular_velocity, double drag, double mass);

void destroy_body(const Body *body);

void body_integrate(Body *body, double dt);

void body_set_position(Body *body, Vector2 position);

void body_set_angle(Body *body, double angle);

void body_accelerate(Body *body, Vector2 acceleration);

void normalize_angle(double *angle);

int is_colliding_circular(const Body *b1, const Body *b2, double r1, double r2);

int is_colliding_sat(const Body *a, const Body *b);

void collision_elastic(Body *a, Body *b);

#endif //ASTEROIDS_PHYSICS_H
