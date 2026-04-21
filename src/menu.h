#ifndef ASTEROIDS_MENU_H
#define ASTEROIDS_MENU_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

typedef enum MenuChoice { PLAY, QUIT } MenuChoice;

typedef struct {
    TTF_Font *title_font;
    TTF_Font *font;

    SDL_Texture *title_texture;
    SDL_Surface *title_surface;

    SDL_Texture *play_active;
    SDL_Texture *play_inactive;
    SDL_Surface *play_surface;

    SDL_Texture *continue_active;
    SDL_Texture *continue_inactive;
    SDL_Surface *continue_surface;

    SDL_Texture *quit_active;
    SDL_Texture *quit_inactive;
    SDL_Surface *quit_surface;

    MenuChoice menu_choice;
} Menu;

void init_menu(Menu *menu, SDL_Renderer *r);

void render_title(const Menu *menu, SDL_Renderer *renderer);

void render_pause(const Menu *menu, SDL_Renderer *renderer);

void destroy_menu(const Menu *menu);

#endif // ASTEROIDS_MENU_H
