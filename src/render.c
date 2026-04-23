#include "render.h"

void draw_polygon(SDL_Renderer *renderer, const Polygon *polygon) {
    for (size_t i = 0; i < polygon->point_count; i++) {
        const size_t j = i + 1 == polygon->point_count ? 0 : i + 1;
        SDL_RenderDrawLine(renderer,
            (int)polygon->world[i].x, (int)polygon->world[i].y,
            (int)polygon->world[j].x, (int)polygon->world[j].y);
    }
}

void wrap_around_screen(Body *body, const int screen_width, const int screen_height) {
    Vector2 pos = body->position;
    if (pos.x < 0) pos.x = screen_width;
    else if (pos.x > screen_width)  pos.x = 0;
    if (pos.y < 0) pos.y = screen_height;
    else if (pos.y > screen_height) pos.y = 0;
    body_set_position(body, pos);
}

