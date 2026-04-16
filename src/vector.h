#pragma once

#include <SDL2/SDL.h>

typedef struct {
    double x;
    double y;
} Vector2;

Vector2 creat_vector2(double x, double y);

Vector2 vector_add(Vector2 a, Vector2 b);

void vector_update(Vector2 *a, const Vector2 *b);

Vector2 vector_rotate(Vector2 vector, double angle);

Vector2 vector_scale(Vector2 vector, double scale);

typedef struct {
    Vector2 *offsets; // points defined relative to centroid
    Vector2 *world; //computed: position + rotated offsets
    size_t point_count;
} Polygon;

Polygon polygon_init(size_t point_count, Vector2 offsets[]);

void draw_polygon(SDL_Renderer *renderer, const Polygon *polygon);

void polygon_destroy(const Polygon *poly);
