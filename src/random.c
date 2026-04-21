#include "random.h"
#include <stdlib.h>
#include <math.h>

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
