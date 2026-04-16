#include <SDL2/SDL_render.h>
#include <math.h>
#include <SDL2/SDL_events.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "config.h"
#include "entities.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    Ship ship;
} GameState;

GameState *init_app(void) {
    const int rendererFlags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
    const int windowFlags = SDL_WINDOW_OPENGL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    GameState *app = calloc(1, sizeof(*app));

    app->window = SDL_CreateWindow(
        "Asteroids",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_WIDTH,
        windowFlags);

    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);

    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    app->ship = init_ship();
    return app;
}

void destroy_app(GameState *app) {
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
    destroy_body(&app->ship.body);
    free(app);
}

void handleKeyboard(Ship *ship, Bullets *bullets) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W]) {
        body_accelerate(&ship->body, (Vector2){
            .x = ACCELERATION_MAGNITUDE * cos(ship->body.angle),
            .y = ACCELERATION_MAGNITUDE * sin(ship->body.angle)
        } );
    }

    if (state[SDL_SCANCODE_A]) {
        ship->body.angle -= ANGLE_MAGNITUDE;
    }

    if (state[SDL_SCANCODE_D]) {
        ship->body.angle += ANGLE_MAGNITUDE;
    }

    if (state[SDL_SCANCODE_SPACE]) {
        const Uint32 now = SDL_GetTicks();
        if (now - bullets->last_shot >= bullets->cooldown) {
            fire_bullet(bullets, ship);
            bullets->last_shot = now;
        }
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

void draw_bullets(SDL_Renderer *renderer, Bullets *bullets) {
    for (int i = 0; i < bullets->count; i++) {
        draw_polygon(renderer, &bullets->bullets[i].body.shape);
        wrap_around_screen(&bullets->bullets[i].body, WINDOW_WIDTH, WINDOW_WIDTH);
    }
}

void draw_asteroid(SDL_Renderer *renderer, Asteroid *a) {
    draw_polygon(renderer, &a->body.shape);
}


int main(void) {
    srand(time(NULL));
    GameState *app = init_app();
    Bullets bullets = {.cooldown = BULLET_COOLDOWN};
    Asteroids asteroids = {0};
    create_asteroids(&asteroids);

    while (1) {
        SDL_Delay(5);
        while (SDL_PollEvent(&app->event)) {
            if (app->event.type == SDL_QUIT) {
                destroy_app(app);
                return EXIT_SUCCESS;
            }
        }

        handleKeyboard(&app->ship, &bullets);

        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);

        body_integrate(&app->ship.body, 1);
        wrap_around_screen(&app->ship.body, WINDOW_WIDTH, WINDOW_WIDTH);
        draw_polygon(app->renderer, &app->ship.body.shape);
        update_bullet(&bullets);
        draw_bullets(app->renderer, &bullets);
        // for (int i = 0; i < asteroids.count; i++) {
        //     Asteroid a = asteroids.asteroids[i];
        //     draw_asteroid(app->renderer, &a);
        //     body_integrate(&a.body, 1);
        //     wrap_around_screen(&a.body, WINDOW_WIDTH, WINDOW_WIDTH);
        // }
        SDL_RenderPresent(app->renderer);
    }

    destroy_app(app);
    return EXIT_SUCCESS;
}