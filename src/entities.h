//
// Created by redonxharja on 9/11/24.
//

#ifndef ENTITIES_H
#define ENTITIES_H

#include <SDL2/SDL_stdinc.h>
#include "config.h"
#include "vector.h"
#include "physics.h"

typedef struct Entity {
  Body body;
  int valid; // tracks lifecycle of entity.
} Entity;

typedef struct Bullet {
  Entity entity;
  double lifetime;
} Bullet;

typedef struct Bullets {
  Bullet bullets[BULLET_COUNT];
  Uint32 cooldown;
  Uint32 last_shot;
} Bullets;

typedef struct Ship {
  Entity entity;
  uint lives;
  double iframes;
} Ship;

typedef struct Particle {
  Entity entity;
  double lifetime;
} Particle;

typedef struct Explosions {
 Particle particles[TOTAL_PARTICLES];
} Explosions;

typedef struct Asteroid {
  Entity entity;
  double radius;
} Asteroid;

typedef struct Asteroids {
  size_t count;
  Asteroid asteroids[ASTEROID_COUNT_MAX];
} Asteroids;


// bullets
void create_bullet(Bullet* b, const Ship *ship, double lifetime);

void fire_bullet(Bullets *bullet_mgr, const Ship *ship);

void degrade_bullet(Bullet *b, double dt);

void destroy_bullets(Bullets *bullets);

// asteroids
Asteroid create_asteroid(double r);

void create_asteroids(Asteroids *asteroids, int count);

void destroy_asteroids(Asteroids *asteroids);

int asteroids_cleared(const Asteroids *asteroids);

// particles
void create_particles(Explosions *explosions, Vector2 position);

void init_explosions(Explosions *explosions);

void free_explosions(Explosions *explosions);

// collisions
void check_asteroids_collision(Asteroids *asteroids);

int check_bullet_collision(Bullets *bullets, Asteroids *asteroids, Explosions *explosions);

// ship
Ship init_ship(void);
int is_ship_colliding(const Ship *ship, const Asteroids *asteroids);

#endif //ENTITIES_H
