#include <math.h>
#include "vector.h"

#include "config.h"
#include "random.h"

Vector2 creat_vector2(const double x, const double y) {
    return (Vector2){ .x = x, .y = y, };
}

Vector2 vector_add(const Vector2 a, const Vector2 b) {
    return creat_vector2(a.x + b.x, a.y + b.y);
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
    const double radius = random_range(r / 2, r);
    return (Vector2) { .x = radius * cos(radian), .y = radius * sin(radian) };
}


Polygon polygon_init(const size_t point_count, Vector2 offsets[]) {
    Polygon polygon = {0};

    polygon.point_count = point_count;
    polygon.offsets = calloc(1, sizeof(Vector2) * point_count);
    polygon.world = calloc(1, sizeof(Vector2) * point_count);

    memcpy(polygon.offsets, offsets, sizeof(Vector2) * point_count);
    return polygon;
}

void polygon_destroy(const Polygon *poly) {
  free(poly->offsets);
  free(poly->world);
}

void draw_polygon(SDL_Renderer *renderer, const Polygon *polygon) {
    for (int i = 0; i < polygon->point_count - 1; i++) {
        const int x1 = (int)polygon->world[i].x;
        const int y1 = (int)polygon->world[i].y;
        const int x2 = (int)polygon->world[i + 1].x;
        const int y2 = (int)polygon->world[i + 1].y;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    // connect the last with the final point
    SDL_RenderDrawLine(renderer,
        (int)polygon->world[polygon->point_count - 1].x,
        (int)polygon->world[polygon->point_count - 1].y,
        (int)polygon->world[0].x,
        (int)polygon->world[0].y
    );
}

Vector2 vector_rotate(const Vector2 vector, const double angle) {
    return creat_vector2(
    cos(angle) * vector.x  - sin(angle) * vector.y,
    sin(angle) * vector.x  + cos(angle) * vector.y
    );
}