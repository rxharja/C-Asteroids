#include <SDL2/SDL_render.h>
#include <math.h>
#include <SDL2/SDL_events.h>
#include <stdlib.h>
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
    polygon_destroy(&app->ship.shape);
    free(app);
}

void handleKeyboard(Ship *ship, Bullets *bullets) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W]) {
        ship->acceleration.x += ACCELERATION_MAGNITUDE * cos(ship->shape.angle);
        ship->acceleration.y += ACCELERATION_MAGNITUDE * sin(ship->shape.angle);
    } else {
        ship->acceleration.x = 0;
        ship->acceleration.y = 0;
    }

    if (state[SDL_SCANCODE_A]) {
        ship->shape.angle -= ANGLE_MAGNITUDE;
        normalizeAngle(&ship->shape.angle);
        update_world(&ship->shape);
    }

    if (state[SDL_SCANCODE_D]) {
        ship->shape.angle += ANGLE_MAGNITUDE;
        normalizeAngle(&ship->shape.angle);
        update_world(&ship->shape);
    }

    if (state[SDL_SCANCODE_SPACE]) {
        const Uint32 now = SDL_GetTicks();
        if (now - bullets->last_shot >= bullets->cooldown) {
            fire_bullet(bullets, ship);
            bullets->last_shot = now;
        }
    }
}

void drag(Vector2 *velocity) {
    velocity->x *= DRAG_FACTOR;
    velocity->y *= DRAG_FACTOR;

    if (fabs(velocity->x) < EPSILON) {
        velocity->x = 0;
    }
    if (fabs(velocity->y) < EPSILON) {
        velocity->y = 0;
    }
}


void wrap_around_screen(Polygon *shape, const int screen_width, const int screen_height) {
    for (int i = 0; i < shape->point_count; i++) {
        // Check X-axis
        if (shape->world[i].x < 0) {
            polygon_translate(shape, (Vector2){screen_width, 0});
        } else if (shape->world[i].x > screen_width) {
            polygon_translate(shape, (Vector2){-screen_width, 0});
        }

        // Check Y-axis
        if (shape->world[i].y < 0) {
            polygon_translate(shape, (Vector2){0, screen_height});
        } else if (shape->world[i].y > screen_height) {
            polygon_translate(shape, (Vector2){0, -screen_height});
        }
    }
}

void draw_bullets(SDL_Renderer *renderer, Bullets *bullets) {
    for (int i = 0; i < bullets->count; i++) {
        draw_polygon(renderer, &bullets->bullets[i].shape);
        wrap_around_screen(&bullets->bullets[i].shape, WINDOW_WIDTH, WINDOW_WIDTH);
    }
}


int main(void) {
    GameState *app = init_app();
    Bullets bullets = {.cooldown = BULLET_COOLDOWN};

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

        move_ship(&app->ship);
        wrap_around_screen(&app->ship.shape, WINDOW_WIDTH, WINDOW_WIDTH);
        drag(&app->ship.velocity);
        draw_polygon(app->renderer, &app->ship.shape);
        update_bullet(&bullets);
        draw_bullets(app->renderer, &bullets);
        SDL_RenderPresent(app->renderer);
    }

    destroy_app(app);
    return EXIT_SUCCESS;
}
