//
// Created by redonxharja on 4/15/26.
//

#include "physics.h"

void update_world(const Body *body) {
    for (int i = 0; i < body->shape.point_count; i++) {
        body->shape.world[i] = vector_add(body->position, vector_rotate(body->shape.offsets[i], body->angle));
    }
}

Body create_body(const Polygon shape, const Vector2 position, const Vector2 velocity, const double angle,
                 const double angular_velocity, const double drag) {
    Body b = {0};
    b.shape = shape;
    b.position = position;
    b.acceleration = (Vector2){0};
    b.velocity = velocity;
    b.angle = angle;
    b.angular_velocity = angular_velocity;
    b.drag = drag;
    return b;
}

// update position, angle, and then world shapes
void body_integrate(Body *body, const double dt) {
    body->velocity = vector_add(body->velocity, vector_scale(body->acceleration, dt));
    body->velocity = vector_scale(body->velocity, pow(body->drag, dt));
    body->position = vector_add(body->position, vector_scale(body->velocity, dt));
    body->angle += body->angular_velocity * dt;
    body->acceleration = (Vector2){0};
    normalize_angle(&body->angle);
    update_world(body);
}

void body_set_position(Body *body, const Vector2 position) {
    body->position = position;
}

void body_set_angle(Body *body, const double angle) {
    body->angle = angle;
}

void body_accelerate(Body *body, const Vector2 acceleration) {
    body->acceleration = vector_add(body->acceleration, acceleration);
}

void destroy_body(const Body *body) {
    polygon_destroy(&body->shape);
}

void normalize_angle(double *angle) {
    if (*angle > 2 * M_PI) {
        *angle -= 2 * M_PI;
    } else if (*angle < 0) {
        *angle += 2 * M_PI;
    }
}