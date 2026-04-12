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

typedef struct {
    Vector2 position; // centroid
    double angle;
    Vector2 *offsets; // points defined relative to centroid
    Vector2 *world; //computed: position + rotated offsets
    size_t point_count;
} Polygon;

Polygon polygon_init(size_t point_count, Vector2 position, Vector2 offsets[], double angle);

void update_world(const Polygon *polygon);

void draw_polygon(SDL_Renderer *renderer, const Polygon *polygon);

void polygon_destroy(const Polygon *poly);

void polygon_translate(Polygon *polygon,  Vector2 velocity);

void normalizeAngle(double *angle);

