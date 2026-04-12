//
// Created by redonxharja on 9/11/24.
//

#ifndef ENTITIES_H
#define ENTITIES_H

#include "vector.h"

typedef struct Bullet {
  polygon shape;
  vector2 velocity;
  double lifetime;
} Bullet;

typedef struct Bullets {
  size_t count;
  Bullet bullets[16];
  Uint32 cooldown;
  Uint32 last_shot;
} Bullets;

typedef struct Ship {
  polygon shape;
  vector2 velocity;
  vector2 acceleration;
  double angle;
} Ship;

void create_bullet(Bullet* b, const Ship *ship, double lifetime);

void fire_bullet(Bullets *bullet_mgr, const Ship *ship);

void update_bullet(Bullets *bullet_mgr);

Ship init_ship(void);

void move_ship(Ship *ship);

#endif //ENTITIES_H
