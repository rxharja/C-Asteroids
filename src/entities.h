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

typedef struct Explosion {
  Entity entity;
  double lifetime;
} Explosion;

typedef struct Explosions {
 Explosion explosions[36];
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

void destroy_bullets(const Bullets *bullets);

// asteroids
Asteroid create_asteroid(double r);

void create_asteroids(Asteroids *asteroids, int count);

void destroy_asteroids(const Asteroids *asteroids);

int asteroids_cleared(const Asteroids *asteroids);

// particles
Explosion create_particles(Vector2 position, double lifetime);

void create_explosions(Explosions *explosions, Vector2 position);

void free_explosions(const Explosions *explosions);

// collisions
void check_asteroids_collision(Asteroids *asteroids);

int check_bullet_collision(Bullets *bullets, Asteroids *asteroids, Explosions *explosions);

// ship
Ship init_ship(void);
int is_ship_colliding(const Ship *ship, const Asteroids *asteroids);

#endif //ENTITIES_H
