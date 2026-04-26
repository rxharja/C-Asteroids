//
// Created by redonxharja on 4/26/26.
//

#ifndef ASTEROIDS_SOUND_H
#define ASTEROIDS_SOUND_H

#include <SDL2/SDL_mixer.h>

typedef enum Beat { LOW, HIGH } Beat;

typedef struct Sound {
    Mix_Chunk *shoot;
    Mix_Chunk *explode;
    Mix_Chunk *low_bass;
    Mix_Chunk *high_bass;
    double beat_timer;
    Beat beat_toggle;
} Sound;

Sound init_sound();

void free_sound(const Sound *sound);

void shoot(const Sound *sound);

void explode(const Sound *sound);

void low_bass(const Sound *sound);

void high_bass(const Sound *sound);

void beat(Sound *sound, double dt, double interval);

#endif //ASTEROIDS_SOUND_H
