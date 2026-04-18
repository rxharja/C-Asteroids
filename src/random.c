//
// Created by redonxharja on 4/17/26.
//

#include "random.h"
#include <stdlib.h>

double random_float() {
    return (double)rand() / (double)RAND_MAX;
}

int random_range(const int min, const int max) {
   return rand() % (max + 1 - min) + min;
}

int random_bool() {
    return random_range(0, 1) == 0;
}

int random_int_choice(const int one, const int two) {
    if (random_bool()) return one;
    return two;
}
