//
// Created by redonxharja on 4/20/26.
//

#include "menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "config.h"

void init_menu(Menu *menu, SDL_Renderer *r) {
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

    const SDL_Color white = {255, 255, 255, 255};
    const SDL_Color green = {0, 255, 0, 255};

    menu->title_surface = TTF_RenderText_Blended(menu->title_font, "ASTEROIDS", white);
    menu->title_texture = SDL_CreateTextureFromSurface(r, menu->title_surface);

    menu->play_surface = TTF_RenderText_Blended(menu->font, "Play", white);
    menu->play_inactive = SDL_CreateTextureFromSurface(r, menu->play_surface);
    SDL_Surface *s = TTF_RenderText_Blended(menu->font, "Play", green);
    menu->play_active = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    menu->continue_surface = TTF_RenderText_Blended(menu->font, "Continue", white);
    menu->continue_inactive = SDL_CreateTextureFromSurface(r, menu->continue_surface);
    s = TTF_RenderText_Blended(menu->font, "Continue", green);
    menu->continue_active = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    menu->quit_surface = TTF_RenderText_Blended(menu->font, "Quit", white);
    menu->quit_inactive = SDL_CreateTextureFromSurface(r, menu->quit_surface);
    s = TTF_RenderText_Blended(menu->font, "Quit", green);
    menu->quit_active = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
}

void render_title(const Menu *menu, SDL_Renderer *renderer) {
    const SDL_Rect title_dest = {
        (WINDOW_WIDTH - menu->title_surface->w)/2,
        (WINDOW_HEIGHT - menu->title_surface->h)/2 - TITLE_OFFSET,
        menu->title_surface->w, menu->title_surface->h
    };
    SDL_RenderCopy(renderer, menu->title_texture, NULL, &title_dest);

    SDL_Texture *play_tex = menu->menu_choice == PLAY ? menu->play_active : menu->play_inactive;
    const SDL_Rect dest1 = {
        (WINDOW_WIDTH - menu->play_surface->w)/2,
        (WINDOW_HEIGHT - menu->play_surface->h)/2,
        menu->play_surface->w, menu->play_surface->h
    };
    SDL_RenderCopy(renderer, play_tex, NULL, &dest1);

    SDL_Texture *quit_tex = menu->menu_choice == QUIT ? menu->quit_active : menu->quit_inactive;
    const SDL_Rect dest2 = {
        (WINDOW_WIDTH - menu->quit_surface->w)/2,
        (WINDOW_HEIGHT - menu->quit_surface->h)/2 + 48,
        menu->quit_surface->w, menu->quit_surface->h
    };
    SDL_RenderCopy(renderer, quit_tex, NULL, &dest2);
}

void render_pause(const Menu *menu, SDL_Renderer *renderer) {
    const SDL_Rect title_dest = {
        (WINDOW_WIDTH - menu->title_surface->w)/2,
        (WINDOW_HEIGHT - menu->title_surface->h)/2 - TITLE_OFFSET,
        menu->title_surface->w, menu->title_surface->h
    };
    SDL_RenderCopy(renderer, menu->title_texture, NULL, &title_dest);

    SDL_Texture *cont_tex = menu->menu_choice == PLAY ? menu->continue_active : menu->continue_inactive;
    const SDL_Rect dest1 = {
        (WINDOW_WIDTH - menu->continue_surface->w)/2,
        (WINDOW_HEIGHT - menu->continue_surface->h)/2,
        menu->continue_surface->w, menu->continue_surface->h
    };
    SDL_RenderCopy(renderer, cont_tex, NULL, &dest1);

    SDL_Texture *quit_tex = menu->menu_choice == QUIT ? menu->quit_active : menu->quit_inactive;
    const SDL_Rect dest2 = {
        (WINDOW_WIDTH - menu->quit_surface->w)/2,
        (WINDOW_HEIGHT - menu->quit_surface->h)/2 + 48,
        menu->quit_surface->w, menu->quit_surface->h
    };
    SDL_RenderCopy(renderer, quit_tex, NULL, &dest2);
}

void destroy_menu(const Menu *menu) {
    SDL_FreeSurface(menu->title_surface);
    SDL_DestroyTexture(menu->title_texture);
    SDL_FreeSurface(menu->play_surface);
    SDL_DestroyTexture(menu->play_active);
    SDL_DestroyTexture(menu->play_inactive);
    SDL_FreeSurface(menu->continue_surface);
    SDL_DestroyTexture(menu->continue_active);
    SDL_DestroyTexture(menu->continue_inactive);
    SDL_FreeSurface(menu->quit_surface);
    SDL_DestroyTexture(menu->quit_active);
    SDL_DestroyTexture(menu->quit_inactive);
    TTF_CloseFont(menu->font);
    TTF_CloseFont(menu->title_font);
}
