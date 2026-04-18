//
// Created by redonxharja on 4/18/26.
//

#ifndef ASTEROIDS_GAMESTATE_H
#define ASTEROIDS_GAMESTATE_H
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#include "entities.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    Ship ship;
    Asteroids asteroids;
    Bullets bullets;
    Uint32 last_tick;
} GameState;

GameState *init_app(void);

void update(GameState *game_state);

void destroy_app(GameState *app);

#endif //ASTEROIDS_GAMESTATE_H
