//
// Created by redonxharja on 4/20/26.
//

#include "menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "config.h"

void init_menu(Menu *menu) {
    if (TTF_Init() < 0) {
        printf("Couldn't initialize SDL_ttf: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    menu->title_font = TTF_OpenFont("./lib/font.ttf", 96);
    menu->font = TTF_OpenFont("./lib/font.ttf", 48);
    if (!menu->font || !menu->title_font) {
        printf("Couldn't load font: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

void render_menu(SDL_Renderer *r, const Menu *m, const char *title, const char *option1, const char *option2) {
    const SDL_Color white = {255, 255, 255, 255};
    const SDL_Color green = {0, 255, 0, 255};
    SDL_Surface *title_surface = TTF_RenderText_Blended(m->title_font, title, white);
    SDL_Texture *title_texture = SDL_CreateTextureFromSurface(r, title_surface);

    SDL_Surface *option1_surface = TTF_RenderText_Blended(m->font, option1, m->menu_choice == OPTION1 ? green : white);
    SDL_Texture *option1_texture = SDL_CreateTextureFromSurface(r, option1_surface);

    SDL_Surface *option2_surface = TTF_RenderText_Blended(m->font, option2, m->menu_choice == OPTION2 ? green : white);
    SDL_Texture *option2_texture = SDL_CreateTextureFromSurface(r, option2_surface);

    const SDL_Rect title_dest = {
        (WINDOW_WIDTH - title_surface->w)/2,
        (WINDOW_HEIGHT - title_surface->h)/2 - TITLE_OFFSET,
        title_surface->w, title_surface->h
    };

    const SDL_Rect dest1 = {
        (WINDOW_WIDTH - option1_surface->w)/2,
        (WINDOW_HEIGHT - option1_surface->h)/2,
        option1_surface->w, option1_surface->h
    };

    const SDL_Rect dest2 = {
        (WINDOW_WIDTH - option2_surface->w)/2,
        (WINDOW_HEIGHT - option2_surface->h)/2 + 48,
        option2_surface->w, option2_surface->h
    };

    SDL_RenderCopy(r, title_texture, NULL, &title_dest);
    SDL_RenderCopy(r, option1_texture, NULL, &dest1);
    SDL_RenderCopy(r, option2_texture, NULL, &dest2);

    SDL_FreeSurface(title_surface);
    SDL_DestroyTexture(title_texture);
    SDL_FreeSurface(option1_surface);
    SDL_DestroyTexture(option1_texture);
    SDL_FreeSurface(option2_surface);
    SDL_DestroyTexture(option2_texture);
}

void render_info(SDL_Renderer *r, const Menu *m, const char *section1, const char *section2) {
    const SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *section1_surface = TTF_RenderText_Blended(m->font, section1, white);
    SDL_Texture *section1_texture = SDL_CreateTextureFromSurface(r, section1_surface);
    SDL_Surface *section2_surface = TTF_RenderText_Blended(m->font, section2, white);
    SDL_Texture *section2_texture = SDL_CreateTextureFromSurface(r, section2_surface);

    const SDL_Rect dest1 = {
        MARGIN_X,
        WINDOW_HEIGHT - section1_surface->h - MARGIN_Y,
        section1_surface->w, section1_surface->h
    };

    const SDL_Rect dest2 = {
        WINDOW_WIDTH - section2_surface->w - MARGIN_X,
        WINDOW_HEIGHT - section2_surface->h - MARGIN_Y,
        section2_surface->w, section2_surface->h
    };

    SDL_RenderCopy(r, section1_texture, NULL, &dest1);
    SDL_RenderCopy(r, section2_texture, NULL, &dest2);

    SDL_FreeSurface(section1_surface);
    SDL_DestroyTexture(section1_texture);
    SDL_FreeSurface(section2_surface);
    SDL_DestroyTexture(section2_texture);
}

void destroy_menu(const Menu *menu) {
    TTF_CloseFont(menu->font);
    TTF_CloseFont(menu->title_font);
}
