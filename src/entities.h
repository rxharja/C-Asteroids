//
// Created by redonxharja on 9/11/24.
//

#ifndef ENTITIES_H
#define ENTITIES_H

#include "vector.h"
#include "physics.h"

typedef struct Bullet {
  Body body;
  double lifetime;
} Bullet;

typedef struct Bullets {
  size_t count;
  Bullet bullets[16];
  Uint32 cooldown;
  Uint32 last_shot;
} Bullets;

typedef struct Ship {
  Body body;
  Vector2 acceleration;
} Ship;

typedef struct Asteroid {
  Body body;
  double radius;
} Asteroid;

typedef struct Asteroids {
  size_t count;
  Asteroid asteroids[16];
} Asteroids;


void create_bullet(Bullet* b, const Ship *ship, double lifetime);

Asteroid create_asteroid(double r);

Asteroids create_asteroids();

void fire_bullet(Bullets *bullet_mgr, const Ship *ship);

void update_bullet(Bullets *bullet_mgr);

Ship init_ship(void);

void move_ship(Ship *ship);

#endif //ENTITIES_H
