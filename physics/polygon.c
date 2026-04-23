#include "polygon.h"
#include <stdlib.h>
#include <string.h>

Polygon polygon_init(const size_t point_count, Vector2 offsets[]) {
    Polygon polygon = {0};
    polygon.point_count = point_count;
    polygon.offsets = calloc(point_count, sizeof(Vector2));
    polygon.world = calloc(point_count, sizeof(Vector2));
    memcpy(polygon.offsets, offsets, sizeof(Vector2) * point_count);
    return polygon;
}

void polygon_destroy(const Polygon *poly) {
    free(poly->offsets);
    free(poly->world);
}

Projection project(const Polygon *p, const Vector2 *axis) {
    double min = vector_dot(*axis, p->world[0]);
    double max = min;

    for (size_t i = 1; i < p->point_count; i++) {
        const double dot = vector_dot(p->world[i], *axis);
        if (dot < min)      min = dot;
        else if (dot > max) max = dot;
    }

    return (Projection){ min, max };
}

int test_axes(const Polygon *a, const Polygon *b) {
    for (size_t i = 0; i < a->point_count; i++) {
        const Vector2 p1 = a->world[i];
        const Vector2 p2 = a->world[i + 1 == a->point_count ? 0 : i + 1];
        const Vector2 edge = vector_sub(p1, p2);
        const Vector2 normal = vector_perpendicular(edge);
        const Projection proj1 = project(a, &normal);
        const Projection proj2 = project(b, &normal);
        if (proj1.max < proj2.min || proj2.max < proj1.min) return 0;
    }
    return 1;
}

Polygon polygon_scale(const Polygon *p, const double factor) {
    Vector2 points[p->point_count];

    for (size_t i = 0; i < p->point_count; i++) {
       points[i] = vector_scale(p->offsets[i], factor);
    }

    return polygon_init(p->point_count, points);
}