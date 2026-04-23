#ifndef PHYSICS_POLYGON_H
#define PHYSICS_POLYGON_H

#include <stddef.h>
#include "vector.h"

typedef struct {
    Vector2 *offsets; // points relative to centroid
    Vector2 *world;   // position + rotated offsets, recomputed each tick
    size_t point_count;
} Polygon;

typedef struct {
    double min;
    double max;
} Projection;

Polygon polygon_init(size_t point_count, Vector2 offsets[]);

void polygon_destroy(const Polygon *poly);

Polygon polygon_scale(const Polygon *p, double factor);

Projection project(const Polygon *p, const Vector2 *axis);

int test_axes(const Polygon *a, const Polygon *b);

#endif // PHYSICS_POLYGON_H
