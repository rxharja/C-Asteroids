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
        Body life = {
            polygon_scale(&ship->entity.body.shape, .25),
             {  60 + 30 * i, 100 }
        };
        body_set_angle(&life, -M_PI_2);
        body_integrate(&life, 1);
        lives[i] = life;
    }
}

void render_score(const GameState *game_state) {
    const Body *first_life = &game_state->lives[0];
    const SDL_Rect dest = {
        (int)first_life->position.x - 15,
        (int)first_life->position.y - 80,
        game_state->score_surface->w,
        game_state->score_surface->h
    };
    SDL_RenderCopy(game_state->renderer, game_state->score_texture, NULL, &dest);
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

static void update_score_counter(GameState *game_state) {
    if (game_state->score_surface != NULL) SDL_FreeSurface(game_state->score_surface);
    if (game_state->score_texture != NULL) SDL_DestroyTexture(game_state->score_texture);
    const SDL_Color white = {255, 255, 255, 255};
    char txt[15];
    snprintf(txt, sizeof(txt),"%i", game_state->score);
    game_state->score_surface = TTF_RenderText_Blended(game_state->menu.font, txt, white);
    game_state->score_texture = SDL_CreateTextureFromSurface(game_state->renderer, game_state->score_surface);
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
    if (s == PAUSE) g->previous_state = g->current_state;
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
    app->score = 0;
    allocate_wave_text(app);
    update_score_counter(app);
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

void try_integrate_entity(Entity *entity, const double dt) {
    if (!entity->valid) return;
    wrap_around_screen(&entity->body, WINDOW_WIDTH, WINDOW_HEIGHT);
    body_integrate(&entity->body, dt);
}

void try_draw_entity(SDL_Renderer *renderer, const Entity *entity) {
    if (!entity->valid) return;
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

    render_score(game_state);
    render_lives(game_state);
    handle_playing_kb(game_state, dt);
    try_integrate_entity(&ship->entity, dt);
    if (ship->iframes <= 0 || (int)(ship->iframes * 10) % 2 == 0 ) try_draw_entity(game_state->renderer, &ship->entity);

    for (int i = 0; i < BULLET_COUNT; i++) {
        try_integrate_entity(&bullets->bullets[i].entity, dt);
        try_draw_entity(game_state->renderer, &bullets->bullets[i].entity);
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
    Ship *ship = &game_state->ship;
    Asteroids *asteroids = &game_state->asteroids;

    move_and_shoot(game_state, dt);
    for (int i = 0; i < ASTEROID_COUNT_MAX; i++) {
        try_integrate_entity(&asteroids->asteroids[i].entity, dt);
        try_draw_entity(game_state->renderer, &asteroids->asteroids[i].entity);
    }

    if (ship->iframes <= 0 && is_ship_colliding(ship, asteroids) ) {
        ship->iframes = IFRAMES;
        ship->lives--;
        if (ship->lives <= 0) set_state(game_state, GAME_OVER);
    }
    ship->iframes -= dt;

    check_asteroids_collision(asteroids);
    const int points = check_bullet_collision(&game_state->bullets, asteroids);

    if (points <= 0) return;
    game_state->score += points;
    update_score_counter(game_state);
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
    SDL_FreeSurface(app->wave_surface);
    SDL_DestroyTexture(app->wave_texture);
    SDL_FreeSurface(app->score_surface);
    SDL_DestroyTexture(app->score_texture);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    TTF_Quit();
    SDL_Quit();
    free(app);
}