#pragma once

typedef struct {
    double x;
    double y;
} Vector2;

Vector2 create_vector2(double x, double y);

Vector2 vector_add(Vector2 a, Vector2 b);

Vector2 vector_sub(Vector2 a, Vector2 b);

void vector_update(Vector2 *a, const Vector2 *b);

Vector2 vector_rotate(Vector2 vector, double angle);

Vector2 vector_scale(Vector2 vector, double scale);

Vector2 random_vector(double r, double radian);

double vector_dot(Vector2 a, Vector2 b);

Vector2 vector_perpendicular(Vector2 v);

Vector2 vector_normal(Vector2 v1, Vector2 v2);

double vector_euclidean_distance(Vector2 v1, Vector2 v2);
