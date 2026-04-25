#include "random.h"
#include <stdlib.h>
#include <math.h>

#include "config.h"

double random_float(void) {
    return (double)rand() / (double)RAND_MAX;
}

int random_range(const int min, const int max) {
    return rand() % (max + 1 - min) + min;
}

int random_bool(void) {
    return random_range(0, 1) == 0;
}

int random_int_choice(const int one, const int two) {
    return random_bool() ? one : two;
}

Vector2 random_vector(const double r, const double radian) {
    const double radius = random_range((int)(r * 0.7), (int)r);
    return (Vector2){ .x = radius * cos(radian), .y = radius * sin(radian) };
}

Vector2 random_edge_position(void) {
    const int edge = rand() % 4;
    switch (edge) {
        case 0: return (Vector2){ random_range(0, WINDOW_WIDTH), 0 };
        case 1: return (Vector2){ random_range(0, WINDOW_WIDTH), WINDOW_HEIGHT };
        case 2: return (Vector2){ 0, random_range(0, WINDOW_HEIGHT) };
        default: case 3: return (Vector2){ WINDOW_WIDTH, random_range(0, WINDOW_HEIGHT) };
    }
}