#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include <time.h>
#include "gamestate.h"

int main(void) {
    srand(time(NULL));
    GameState *app = init_app();

    while (1) {
        while (SDL_PollEvent(&app->event)) {
            if (app->event.type == SDL_QUIT) goto cleanup;
            if (app->event.type == SDL_KEYDOWN) {
                handle_keydown(app, app->event.key.keysym.scancode);
            }
        }

        handle_game_state(app);
    }

cleanup:
    destroy_app(app);
    return EXIT_SUCCESS;
}