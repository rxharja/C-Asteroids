//
// Created by redonxharja on 4/19/26.
//

#include "polygon.h"

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

Projection project(const Polygon *p, const Vector2 *axis) {
    double min = vector_dot(*axis, p->world[0]);
    double max = min;

    for (int i = 1; i < p->point_count; i++) {
        const double dot = vector_dot(p->world[i], *axis);
        if (dot < min)      min = dot;
        else if (dot > max) max = dot;
    }

    return (Projection) { min, max };
}

int test_axes(const Polygon *a, const Polygon *b) {
    for (int i = 0; i < a->point_count; i++) {
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