#pragma once

#include <SDL2/SDL.h>

typedef struct {
    double x;
    double y;
} vector2;

vector2 creat_vector2(double x, double y);

void vector_add(vector2 *a, const vector2 *b);

void vector_rotate(vector2 *vector, double angle);

typedef struct polygonal_shape {
    vector2 *points;
    size_t point_count;

} polygon;

polygon polygon_init(size_t point_count, vector2 vectors[]);

void draw_polygon(SDL_Renderer *renderer, const polygon *polygon);

void polygon_destroy(const polygon *poly);

vector2 polygon_centroid(const polygon *polygon);

void polygon_translate(const polygon *polygon, const vector2 *velocity);

void polygon_rotate(const polygon *polygon, double angle);

void polygon_rotate_around(vector2 centroid, const polygon *polygon, double angle);
