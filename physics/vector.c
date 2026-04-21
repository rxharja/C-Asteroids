#include "vector.h"
#include <math.h>

// velocities below this magnitude snap to zero so tiny residual drift
// doesn't outlive a stop
static const double VECTOR_EPSILON = 0.02;

Vector2 vector_add(const Vector2 a, const Vector2 b) {
    return (Vector2){ .x = a.x + b.x, .y = a.y + b.y };
}

Vector2 vector_sub(const Vector2 a, const Vector2 b) {
    return (Vector2){ .x = a.x - b.x, .y = a.y - b.y };
}

Vector2 vector_scale(Vector2 vector, const double scale) {
    vector.x *= scale;
    vector.y *= scale;
    if (fabs(vector.x) < VECTOR_EPSILON) vector.x = 0;
    if (fabs(vector.y) < VECTOR_EPSILON) vector.y = 0;
    return vector;
}

Vector2 vector_rotate(const Vector2 v, const double angle) {
    const double c = cos(angle);
    const double s = sin(angle);
    return (Vector2){ .x = c * v.x - s * v.y, .y = s * v.x + c * v.y };
}

double vector_dot(const Vector2 a, const Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

Vector2 vector_perpendicular(const Vector2 v) {
    return (Vector2){ .x = -v.y, .y = v.x };
}

double vector_euclidean_distance(const Vector2 v1, const Vector2 v2) {
    const double dx = v1.x - v2.x;
    const double dy = v1.y - v2.y;
    return sqrt(dx * dx + dy * dy);
}

Vector2 vector_normal(const Vector2 v1, const Vector2 v2) {
    const double d = vector_euclidean_distance(v1, v2);
    if (d < VECTOR_EPSILON) return (Vector2){0};
    return (Vector2){ .x = (v1.x - v2.x) / d, .y = (v1.y - v2.y) / d };
}
