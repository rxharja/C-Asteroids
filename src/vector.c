#include <math.h>
#include "vector.h"
#include "config.h"
#include "random.h"

Vector2 create_vector2(const double x, const double y) {
    return (Vector2){ .x = x, .y = y, };
}

Vector2 vector_add(const Vector2 a, const Vector2 b) {
    return create_vector2(a.x + b.x, a.y + b.y);
}

Vector2 vector_sub(const Vector2 a, const Vector2 b) {
    return create_vector2(a.x - b.x, a.y - b.y);
}

void vector_update(Vector2 *a, const Vector2 *b) {
    a->x += b->x;
    a->y += b->y;
}

Vector2 vector_scale(Vector2 vector, const double scale) {
    vector.x *= scale;
    vector.y *= scale;
    if (fabs(vector.x) < EPSILON) vector.x = 0;
    if (fabs(vector.y) < EPSILON) vector.y = 0;
    return vector;
}

Vector2 random_vector(const double r, const double radian) {
    const double radius = random_range(r * .7, r);
    return (Vector2) { .x = radius * cos(radian), .y = radius * sin(radian) };
}

double vector_dot(const Vector2 a, const Vector2 b) {
   return a.x * b.x + a.y * b.y;
}

Vector2 vector_rotate(const Vector2 vector, const double angle) {
    return create_vector2(
    cos(angle) * vector.x  - sin(angle) * vector.y,
    sin(angle) * vector.x  + cos(angle) * vector.y
    );
}

Vector2 vector_perpendicular(const Vector2 v) {
    return (Vector2) { .x = -v.y, .y = v.x };
}

double vector_euclidean_distance(const Vector2 v1, const Vector2 v2) {
    const double dx = v1.x - v2.x;
    const double dy = v1.y - v2.y;
    return sqrt(dx * dx + dy * dy);
}

Vector2 vector_normal(const Vector2 v1, const Vector2 v2) {
    const double d = vector_euclidean_distance(v1, v2);
    if (d < EPSILON) return (Vector2) { .x = 0, .y = 0 };
    return (Vector2) { .x = (v1.x - v2.x) /d , .y = (v1.y - v2.y) /d };
}
