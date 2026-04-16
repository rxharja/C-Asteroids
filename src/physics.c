//
// Created by redonxharja on 4/15/26.
//


#include "physics.h"

Body create_body(const Polygon polygon, const Vector2 velocity) {
    Body b = {0};
    b.shape = polygon;
    b.velocity = velocity;
    return b;
}

void move_body(Body *body) {
    polygon_translate(&body->shape, body->velocity);
}

void destroy_body(const Body *body) {
    polygon_destroy(&body->shape);
    // free(body);
}
