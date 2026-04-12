#include <math.h>
#include "vector.h"

vector2 creat_vector2(const double x, const double y) {
    return (vector2){ .x = x, .y = y, };
}

void vector_add(vector2 *a, const vector2 *b) {
    a->x += b->x;
    a->y += b->y;
}

polygon polygon_init(const size_t point_count, vector2 vectors[]) {
    polygon polygon = {0};

    polygon.point_count = point_count;

    polygon.points = malloc(sizeof(vector2) * point_count);
    memset(polygon.points, 0, sizeof(vector2) * point_count);

    for (int i = 0; i < point_count; i++) {
        polygon.points[i] = vectors[i];
    }

    return polygon;
}

void polygon_destroy(const polygon *poly) {
  free(poly->points);
}

void draw_polygon(SDL_Renderer *renderer, const polygon *polygon) {
    for (int i = 0; i < polygon->point_count - 1; i++) {
        const int x1 = (int)polygon->points[i].x;
        const int y1 = (int)polygon->points[i].y;
        const int x2 = (int)polygon->points[i + 1].x;
        const int y2 = (int)polygon->points[i + 1].y;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    // connect the last with the final point
    SDL_RenderDrawLine(renderer,
        (int)polygon->points[polygon->point_count - 1].x,
        (int)polygon->points[polygon->point_count - 1].y,
        (int)polygon->points[0].x,
        (int)polygon->points[0].y
    );
}

void polygon_translate(const polygon *polygon, const vector2 *velocity) {
    for (int i = 0; i < polygon->point_count; i++) {
        polygon->points[i].x += velocity->x;
        polygon->points[i].y += velocity->y;
    }
}

// calculates the center point of the polygon using the averages of the x and y vectors
vector2 polygon_centroid(const polygon *polygon) {
    double x_total = 0;
    double y_total = 0;

    for (int i = 0; i < polygon->point_count; i++) {
        x_total += polygon->points[i].x;
        y_total += polygon->points[i].y;
    }

    return (vector2){
        .x = x_total / (double) polygon->point_count, .y = y_total / (double) polygon->point_count
    };
}

void vector_rotate(vector2 *vector, const double angle) {
    const double x = vector->x;
    const double y = vector->y;
    vector->x = cos(angle) * x  - sin(angle) * y;
    vector->y = sin(angle) * x  + cos(angle) * y;
}

void polygon_rotate(const polygon *polygon, const double angle) {
    polygon_rotate_around(polygon_centroid(polygon),polygon, angle);
}

void polygon_rotate_around(const vector2 centroid, const polygon *polygon, const double angle) {
    const double cos_ = cos(angle);
    const double sin_ = sin(angle);
    for (int i = 0; i < polygon->point_count; i++) {
        const double x = polygon->points[i].x;
        const double y = polygon->points[i].y;
        polygon->points[i].x = (cos_ * (x - centroid.x) - sin_ * (y - centroid.y)) + centroid.x;
        polygon->points[i].y = (sin_ * (x - centroid.x) + cos_ * (y - centroid.y)) + centroid.y;
    }
}
