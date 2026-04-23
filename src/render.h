#ifndef ASTEROIDS_RENDER_H
#define ASTEROIDS_RENDER_H

#include <SDL2/SDL_render.h>

#include "physics.h"
#include "polygon.h"

void draw_polygon(SDL_Renderer *renderer, const Polygon *polygon);

void wrap_around_screen(Body *body, int screen_width, int screen_height);

#endif // ASTEROIDS_RENDER_H
