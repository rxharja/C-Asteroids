//
// Created by redonxharja on 4/19/26.
//

#ifndef ASTEROIDS_POLYGON_H
#define ASTEROIDS_POLYGON_H
#include <SDL2/SDL_render.h>

#include "vector.h"

typedef struct {
    Vector2 *offsets; // points defined relative to centroid
    Vector2 *world; //computed: position + rotated offsets
    size_t point_count;
} Polygon;

typedef struct {
    double min;
    double max;
} Projection;

Polygon polygon_init(size_t point_count, Vector2 offsets[]);

void draw_polygon(SDL_Renderer *renderer, const Polygon *polygon);

Projection project(const Polygon *p, const Vector2 *axis);

int test_axes(const Polygon *a, const Polygon *b);

void polygon_destroy(const Polygon *poly);

#endif //ASTEROIDS_POLYGON_H
