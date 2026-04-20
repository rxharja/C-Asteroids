//
// Created by redonxharja on 4/15/26.
//

#include "physics.h"

#include "polygon.h"

void update_world(const Body *body) {
    for (int i = 0; i < body->shape.point_count; i++) {
        body->shape.world[i] = vector_add(body->position, vector_rotate(body->shape.offsets[i], body->angle));
    }
}

Body create_body(const Polygon shape, const Vector2 position, const Vector2 velocity, const double angle,
                 const double angular_velocity, const double drag, const double mass) {
    Body b = {0};
    b.shape = shape;
    b.position = position;
    b.acceleration = (Vector2){0};
    b.velocity = velocity;
    b.angle = angle;
    b.angular_velocity = angular_velocity;
    b.drag = drag;
    b.mass = mass;
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

int is_colliding_circular(const Body *b1, const Body *b2, const double r1, const double r2) {
    const double dx = b1->position.x - b2->position.x;
    const double dy = b1->position.y - b2->position.y;
    return dx*dx + dy*dy < r1*r1 + r2*r2;
}

int is_colliding_sat(const Body *a, const Body *b) {
    if (!test_axes(&a->shape, &b->shape)) return 0;  // test A's edge normals
    if (!test_axes(&b->shape, &a->shape)) return 0;  // test B's edge normals
    return 1;
}

void collision_elastic(Body *a, Body *b) {
    const double total_mass = a->mass + b->mass;
    const Vector2 normal = vector_normal(a->position, b->position);
    const double proj_a = vector_dot(a->velocity, normal);
    const double proj_b = vector_dot(b->velocity, normal);
    a->position = vector_add(a->position, normal);
    b->position = vector_sub(b->position, normal);
    a->velocity = vector_add(
        vector_sub(a->velocity, vector_scale(normal, proj_a)),
        vector_scale(normal, (2 * b->mass * proj_b + (a->mass - b->mass) * proj_a) / total_mass));
    b->velocity = vector_add(
        vector_sub(b->velocity, vector_scale(normal, proj_b)),
        vector_scale(normal, (2 * a->mass * proj_a + (b->mass - a->mass) * proj_b) / total_mass));}


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
