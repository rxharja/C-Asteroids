#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "gamestate.h"


int main(void) {
    srand(time(NULL));
    GameState *app = init_app();

    while (1) {
        SDL_Delay(5);

        while (SDL_PollEvent(&app->event)) {
            if (app->event.type != SDL_QUIT) continue;
            goto cleanup;
        }

        update(app);
    }

cleanup:
    destroy_app(app);
    return EXIT_SUCCESS;
}