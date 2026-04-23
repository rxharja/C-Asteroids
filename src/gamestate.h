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

#define STATE_STACK_MAX 4

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;

    uint wave;
    Uint32 paused_time;
    Uint32 pause_remaining;

    Ship ship;
    Asteroids asteroids;
    Bullets bullets;
    Body lives[LIVES];
    uint score;

    State previous_state; // necessary for switching back when pausing
    State current_state;
    Uint32 last_tick;

    SDL_Texture *wave_texture;
    SDL_Surface *wave_surface;

    SDL_Texture *score_texture;
    SDL_Surface *score_surface;

    Menu menu;
} GameState;

GameState *init_app(void);

void handle_keydown(GameState *game, SDL_Scancode key);

void handle_game_state(GameState *state);

void play(GameState *game_state, double dt);

void destroy_entities(const GameState *app);

void destroy_app(GameState *app);

#endif //ASTEROIDS_GAMESTATE_H
