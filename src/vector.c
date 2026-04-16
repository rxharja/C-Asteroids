#include <math.h>
#include "vector.h"

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

void update_world(const Polygon *polygon) {
    for (int i = 0; i < polygon->point_count; i++) {
        polygon->world[i] = vector_add(polygon->position, vector_rotate(polygon->offsets[i], polygon->angle));
    }
}

Polygon polygon_init(const size_t point_count, const Vector2 position, Vector2 offsets[], const double angle) {
    Polygon polygon = {0};

    polygon.position = position;
    polygon.point_count = point_count;
    polygon.angle = angle;
    polygon.offsets = calloc(1, sizeof(Vector2) * point_count);
    polygon.world = calloc(1, sizeof(Vector2) * point_count);

    memcpy(polygon.offsets, offsets, sizeof(Vector2) * point_count);
    update_world(&polygon);

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

void polygon_translate(Polygon *polygon, const Vector2 velocity) {
    polygon->position = vector_add(polygon->position, velocity);
    update_world(polygon);
}

Vector2 vector_rotate(const Vector2 vector, const double angle) {
    return creat_vector2(
    cos(angle) * vector.x  - sin(angle) * vector.y,
    sin(angle) * vector.x  + cos(angle) * vector.y
    );
}

void normalizeAngle(double *angle) {
    if (*angle > 2 * M_PI) {
        *angle -= 2 * M_PI;
    } else if (*angle < 0) {
        *angle += 2 * M_PI;
    }
}

