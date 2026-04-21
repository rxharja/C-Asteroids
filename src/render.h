#ifndef ASTEROIDS_RENDER_H
#define ASTEROIDS_RENDER_H

#include <SDL2/SDL_render.h>
#include "polygon.h"

void draw_polygon(SDL_Renderer *renderer, const Polygon *polygon);

#endif // ASTEROIDS_RENDER_H
