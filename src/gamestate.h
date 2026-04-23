//
// Created by redonxharja on 4/18/26.
//

#ifndef ASTEROIDS_GAMESTATE_H
#define ASTEROIDS_GAMESTATE_H
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"
#include "entities.h"

typedef enum State {
    TITLE,
    PAUSE,
    PLAYING,
    SPAWN_NEXT,
    MID_WAVE_PAUSE,
    GAME_OVER,
} State;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;

    int wave;
    Uint32 paused_time;

    Ship ship;
    Asteroids asteroids;
    Bullets bullets;

    State state;
    Uint32 last_tick;
    Menu menu;
    int started;
} GameState;

GameState *init_app(void);

void handle_keydown(GameState *game, SDL_Scancode key);

void handle_game_state(GameState *state);

void play(GameState *game_state, double dt);

void destroy_entities(const GameState *app);

void destroy_app(GameState *app);

#endif //ASTEROIDS_GAMESTATE_H
