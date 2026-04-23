//
// Created by redonxharja on 4/18/26.
//

#include <SDL2/SDL.h>
#include "gamestate.h"
#include "menu.h"
#include "config.h"
#include "render.h"

void init_lives(Body lives[LIVES], const Ship *ship) {
    for (int i = 0; i < ship->lives; i++) {
        lives[i] = (Body){
            polygon_scale(&ship->entity.body.shape, .4),
             { 100, WINDOW_HEIGHT - (LIVES * 100)/(i+1) }
        };
        body_integrate(&lives[i], 1);
    }
}

void render_lives(const GameState *game_state) {
    for (int i = 0; i < game_state->ship.lives; i++) {
        draw_polygon(game_state->renderer, &game_state->lives[i].shape);
    }
}

static void allocate_wave_text(GameState *game_state) {
    const SDL_Color white = {255, 255, 255, 255};

    char txt[24];
    snprintf(txt, sizeof(txt),"WAVE %i", game_state->wave + 1);

    game_state->wave_surface = TTF_RenderText_Blended(game_state->menu.title_font, txt, white);
    game_state->wave_texture = SDL_CreateTextureFromSurface(game_state->renderer, game_state->wave_surface);
}

static State current_state(const GameState *g) {
    return g->current_state;
}

static void enter_state(GameState *g, const State s) {
    switch (s) {
        case MID_WAVE_PAUSE:
            g->paused_time = SDL_GetTicks();
            break;
        default:
            break;
    }
}

static void set_state(GameState *g, const State s) {
    if (current_state(g) == s) return;
    g->previous_state = g->current_state;
    g->current_state = s;
    enter_state(g, s);
}

static void init_game_props(GameState *app) {
    app->wave = 0;
    app->asteroids = (Asteroids){ .count = ASTEROID_COUNT_MAX };
    create_asteroids(&app->asteroids, BASE_ASTEROID_SPAWN_COUNT + app->wave);
    app->bullets = (Bullets){ .cooldown = BULLET_COOLDOWN };
    app->ship = init_ship();
    app->last_tick = SDL_GetTicks();
    allocate_wave_text(app);
    init_lives(app->lives, &app->ship);
    set_state(app, TITLE);
}

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

    init_menu(&app->menu, app->renderer);
    init_game_props(app);

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

void handle_keydown(GameState *game, const SDL_Scancode key) {
    switch (current_state(game)) {
        case TITLE:
        case PAUSE:
            if (key == SDL_SCANCODE_W || key == SDL_SCANCODE_UP)
                game->menu.menu_choice = !game->menu.menu_choice;
            if (key == SDL_SCANCODE_S || key == SDL_SCANCODE_DOWN)
                game->menu.menu_choice = !game->menu.menu_choice;
            if (key == SDL_SCANCODE_RETURN && game->menu.menu_choice == PLAY) {
                if (current_state(game) == PAUSE) set_state(game, game->previous_state);
                else set_state(game, MID_WAVE_PAUSE); // initial state after title
            }
            if (key == SDL_SCANCODE_RETURN && game->menu.menu_choice == QUIT) {
                destroy_app(game);
                exit(0);
            }
            break;
        case PLAYING:
            if (key == SDL_SCANCODE_ESCAPE) set_state(game, PAUSE);
            break;
        default:
            break;
    }
}

void handle_playing_kb(GameState *game, const double dt) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    Bullets *bullets = &game->bullets;
    Ship *ship = &game->ship;
    Body *ship_body = &ship->entity.body;

    if (state[SDL_SCANCODE_ESCAPE]) {
        set_state(game, PAUSE);
    }

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

static double update_ticks(GameState *state) {
    const Uint32 now = SDL_GetTicks();
    const double dt = (now - state->last_tick) / 1000.0;
    state->last_tick = now;
    return dt;
}

static void check_pause_time(GameState *state) {
    if (current_state(state) != MID_WAVE_PAUSE) return;

    const Uint32 now = SDL_GetTicks();
    const Uint32 elapsed = now - state->paused_time;

    if (elapsed < PAUSE_TIME) return;
    set_state(state, SPAWN_NEXT);
}

static void move_and_shoot(GameState *game_state, const double dt) {
    Ship *ship = &game_state->ship;
    Bullets *bullets = &game_state->bullets;

    render_lives(game_state);
    handle_playing_kb(game_state, dt);
    try_update_entity(game_state->renderer, &ship->entity, dt);

    for (int i = 0; i < BULLET_COUNT; i++) {
        try_update_entity(game_state->renderer, &bullets->bullets[i].entity, dt);
        degrade_bullet(&bullets->bullets[i], dt);
    }
}

static void mid_wave_pause(GameState *game_state, const double dt) {
    const SDL_Rect wave_dest = {
        (WINDOW_WIDTH - game_state->wave_surface->w)/2,
        (WINDOW_HEIGHT - game_state->wave_surface->h)/2 - WAVE_TEXT_OFFSET,
        game_state->wave_surface->w, game_state->wave_surface->h
    };
    SDL_RenderCopy(game_state->renderer, game_state->wave_texture, NULL, &wave_dest);

    move_and_shoot(game_state, dt);
}

static void reset_state(GameState *state) {
    destroy_entities(state);
    init_game_props(state);
}

void handle_game_state(GameState *state) {
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    const double dt = update_ticks(state);

    switch (current_state(state)) {
        case TITLE:
            render_title(&state->menu, state->renderer);
            break;
        case PAUSE:
            render_pause(&state->menu, state->renderer);
            break;
        case PLAYING:
            play(state, dt);
            if (asteroids_cleared(&state->asteroids)) {
                if (state->wave_surface != NULL) SDL_FreeSurface(state->wave_surface);
                if (state->wave_texture != NULL) SDL_DestroyTexture(state->wave_texture);
                state->wave++;
                allocate_wave_text(state);
                set_state(state, MID_WAVE_PAUSE);
            }
            break;
        case MID_WAVE_PAUSE:
            mid_wave_pause(state, dt);
            check_pause_time(state); // updates pause time, if PAUSE_TIME exceeded, sets state to SPAWN_NEXT
            break;
        case SPAWN_NEXT:
            create_asteroids(&state->asteroids, BASE_ASTEROID_SPAWN_COUNT + state->wave);
            set_state(state, PLAYING);
            break;
        case GAME_OVER:
            if (state->lives <= 0) reset_state(state);
            break;
    }

    SDL_RenderPresent(state->renderer);
}

void play(GameState *game_state, const double dt) {
    Asteroids *asteroids = &game_state->asteroids;

    move_and_shoot(game_state, dt);
    for (int i = 0; i < ASTEROID_COUNT_MAX; i++) {
        try_update_entity(game_state->renderer, &asteroids->asteroids[i].entity, dt);
    }

    if (is_ship_colliding(&game_state->ship, asteroids) && game_state->ship.iframes <= 0) {
        game_state->ship.iframes = 1;
        game_state->ship.lives--;
        if (game_state->ship.lives <= 0) {
            set_state(game_state, GAME_OVER);
        }
    }

    check_asteroids_collision(asteroids);
    check_bullet_collision(&game_state->bullets, asteroids);
}

void destroy_entities(const GameState *app) {
    destroy_bullets(&app->bullets);
    destroy_asteroids(&app->asteroids);
    destroy_body(&app->ship.entity.body);
    for (int i = 0; i < LIVES; i++) {
       destroy_body(&app->lives[i]);
    }
}

void destroy_app(GameState *app) {
    destroy_entities(app);
    destroy_menu(&app->menu);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    TTF_Quit();
    SDL_Quit();
    free(app);
}