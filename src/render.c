#include "render.h"

void draw_polygon(SDL_Renderer *renderer, const Polygon *polygon) {
    for (size_t i = 0; i < polygon->point_count; i++) {
        const size_t j = i + 1 == polygon->point_count ? 0 : i + 1;
        SDL_RenderDrawLine(renderer,
            (int)polygon->world[i].x, (int)polygon->world[i].y,
            (int)polygon->world[j].x, (int)polygon->world[j].y);
    }
}
