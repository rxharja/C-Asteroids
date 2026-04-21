#ifndef PHYSICS_VECTOR_H
#define PHYSICS_VECTOR_H

typedef struct {
    double x;
    double y;
} Vector2;

Vector2 vector_add(Vector2 a, Vector2 b);

Vector2 vector_sub(Vector2 a, Vector2 b);

Vector2 vector_scale(Vector2 vector, double scale);

Vector2 vector_rotate(Vector2 vector, double angle);

double vector_dot(Vector2 a, Vector2 b);

Vector2 vector_perpendicular(Vector2 v);

Vector2 vector_normal(Vector2 v1, Vector2 v2);

double vector_euclidean_distance(Vector2 v1, Vector2 v2);

#endif // PHYSICS_VECTOR_H
