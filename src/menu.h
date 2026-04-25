#ifndef ASTEROIDS_MENU_H
#define ASTEROIDS_MENU_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

typedef enum MenuChoice { OPTION1, OPTION2 } MenuChoice;

typedef struct {
    TTF_Font *title_font;
    TTF_Font *font;
    MenuChoice menu_choice;
} Menu;

void init_menu(Menu *menu);

void render_menu(SDL_Renderer *r, const Menu *m, const char *title, const char *option1, const char *option2);

void render_info(SDL_Renderer *r, const Menu *m, const char *section1, const char *section2);

void destroy_menu(const Menu *menu);

#endif // ASTEROIDS_MENU_H
