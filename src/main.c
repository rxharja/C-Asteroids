#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include <time.h>
#include "gamestate.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static void game_frame(void *arg) {
    GameState *state = arg;
    while (SDL_PollEvent(&state->event)) {
        if (state->event.type == SDL_QUIT) {
            destroy_app(state);
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
            return;
        }
        if (state->event.type == SDL_KEYDOWN) {
            handle_keydown(state, state->event.key.keysym.scancode);
        }
    }
    handle_game_state(state);
}

int main(void) {
    srand(time(NULL));
    GameState *app = init_app();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(game_frame, app, 0, 1);
#else
    while (1) {
        game_frame(app);
    }
#endif
}