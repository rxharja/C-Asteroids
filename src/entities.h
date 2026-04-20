//
// Created by redonxharja on 9/11/24.
//

#ifndef ENTITIES_H
#define ENTITIES_H

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
} Ship;

typedef struct Asteroid {
  Entity entity;
  double radius;
} Asteroid;

typedef struct Asteroids {
  size_t count;
  Asteroid asteroids[ASTEROID_COUNT];
} Asteroids;


// bullets
void create_bullet(Bullet* b, const Ship *ship, double lifetime);

void fire_bullet(Bullets *bullet_mgr, const Ship *ship);

void degrade_bullet(Bullet *b, double dt);

void destroy_bullets(const Bullets *bullets);

// asteroids
Asteroid create_asteroid(double r);

void create_asteroids(Asteroids *asteroids);

void destroy_asteroids(const Asteroids *asteroids);

void check_asteroids_collision(Asteroids *asteroids);

// ship
Ship init_ship(void);

#endif //ENTITIES_H
