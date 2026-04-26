//
// Created by redonxharja on 4/26/26.
//

#include "Sound.h"

Sound init_sound() {
   Sound sound = {0};

   if (Mix_Init(0) != 0) {
      perror("Mix_Init");
   }

   if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
      perror("Mix_OpenAudio");
   }

   sound.shoot = Mix_LoadWAV("sound/shoot.wav");
   sound.explode = Mix_LoadWAV("sound/explode.wav");
   sound.low_bass = Mix_LoadWAV("sound/low_bass.wav");
   sound.high_bass = Mix_LoadWAV("sound/high_bass.wav");

   return sound;
}

void shoot(const Sound *sound) {
   Mix_PlayChannel(-1, sound->shoot, 0);
}

void explode(const Sound *sound) {
   Mix_PlayChannel(-1, sound->explode, 0);
}

void low_bass(const Sound *sound) {
   Mix_PlayChannel(-1, sound->low_bass, 0);
}

void high_bass(const Sound *sound) {
   Mix_PlayChannel(-1, sound->high_bass, 0);
}

void free_sound(const Sound *sound) {
   Mix_FreeChunk(sound->shoot);
   Mix_FreeChunk(sound->explode);
   Mix_FreeChunk(sound->low_bass);
   Mix_FreeChunk(sound->high_bass);
   Mix_CloseAudio();
}
