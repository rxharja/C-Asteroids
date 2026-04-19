//
// Created by redonxharja on 4/18/26.
//

#include "gamestate.h"
#include "config.h"

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
        WINDOW_HEIGHT,
        windowFlags);

    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    app->asteroids = (Asteroids){ .count = ASTEROID_COUNT };
    create_asteroids(&app->asteroids);
    app->bullets = (Bullets){ .cooldown = BULLET_COOLDOWN };
    app->ship = init_ship();
    app-> last_tick = SDL_GetTicks();

    return app;
}

void wrap_around_screen(Body *body, const int screen_width, const int screen_height) {
    Vector2 pos = body->position;
    if (pos.x < 0) pos.x = screen_width;
    else if (pos.x > screen_width)  pos.x = 0;
    if (pos.y < 0) pos.y = screen_height;
    else if (pos.y > screen_height) pos.y = 0;
    body_set_position(body, pos);
}

void try_update_entity(SDL_Renderer *renderer, Entity *entity, const double dt) {
    if (!entity->valid) return;
    wrap_around_screen(&entity->body, WINDOW_WIDTH, WINDOW_HEIGHT);
    body_integrate(&entity->body, dt);
    draw_polygon(renderer, &entity->body.shape);
}

void handleKeyboard(Ship *ship, Bullets *bullets, const double dt) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    Body *ship_body = &ship->entity.body;

    if (state[SDL_SCANCODE_W]) {
        body_accelerate(ship_body, (Vector2){
            .x = ACCELERATION_MAGNITUDE * cos(ship_body->angle),
            .y = ACCELERATION_MAGNITUDE * sin(ship_body->angle)
        } );
    }

    if (state[SDL_SCANCODE_A]) ship_body->angle -= dt * ANGLE_MAGNITUDE;

    if (state[SDL_SCANCODE_D]) ship_body->angle += dt * ANGLE_MAGNITUDE;

    if (state[SDL_SCANCODE_SPACE]) {
        const Uint32 now = SDL_GetTicks();
        if (now - bullets->last_shot >= bullets->cooldown) {
            fire_bullet(bullets, ship);
            bullets->last_shot = now;
        }
    }
}

double update_ticks(GameState *state) {
    const Uint32 now = SDL_GetTicks();
    const double dt = (now - state->last_tick) / 1000.0;
    state->last_tick = now;
    return dt;
}

void update(GameState *game_state) {
    SDL_SetRenderDrawColor(game_state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game_state->renderer);
    SDL_SetRenderDrawColor(game_state->renderer, 255, 255, 255, 255);

    const double dt = update_ticks(game_state);
    Ship *ship = &game_state->ship;
    Bullets *bullets = &game_state->bullets;
    Asteroids *asteroids = &game_state->asteroids;

    handleKeyboard(ship, bullets, dt);

    try_update_entity(game_state->renderer, &ship->entity, dt);

    for (int i = 0; i < BULLET_COUNT; i++) {
        try_update_entity(game_state->renderer, &bullets->bullets[i].entity, dt);
        degrade_bullet(&bullets->bullets[i], dt);
    }

    for (int i = 0; i < ASTEROID_COUNT; i++) try_update_entity(game_state->renderer, &asteroids->asteroids[i].entity, dt);

    SDL_RenderPresent(game_state->renderer);
}

void destroy_app(GameState *app) {
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
    destroy_bullets(&app->bullets);
    destroy_asteroids(&app->asteroids);
    destroy_body(&app->ship.entity.body);
    free(app);
}

