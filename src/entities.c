//
// Created by redonxharja on 9/11/24.
//
#include "entities.h"

#include "config.h"

void create_bullet(Bullet *b, const Ship *ship, const double lifetime) {
  const Vector2 nose = ship->shape.offsets[0];
  const Vector2 stop = {.x = nose.x + BULLET_SIZE, .y = nose.y };

  b->shape = polygon_init(2, ship->shape.position, (Vector2[]){nose,stop}, ship->shape.angle);
  b->velocity.x = 5 * cos(ship->shape.angle);
  b->velocity.y = 5 * sin(ship->shape.angle);
  b->lifetime = lifetime;
}

Ship init_ship(void) {
  Ship ship = {0};

  const Vector2 position = { .x = 300, .y = 470 };
  const Vector2 p1 = { .x = 70,  .y = 0 };    // nose — now +X
  const Vector2 p2 = { .x = -30, .y = -50 };
  const Vector2 p3 = { .x = -5,  .y = -25 };
  const Vector2 p4 = { .x = -5,  .y = 25 };
  const Vector2 p5 = { .x = -30, .y = 50 };

  ship.shape = polygon_init(5, position, (Vector2[]){p1,p2,p3,p4,p5}, -M_PI / 2 );
  ship.velocity = (Vector2){ .x = 0, .y = 0 };
  ship.acceleration = (Vector2){ .x = 0, .y = 0 };

  return ship;
}

void move_ship(Ship *ship) {
  ship->velocity = vector_add(ship->velocity, ship->acceleration);
  polygon_translate(&ship->shape, ship->velocity);
}

void fire_bullet(Bullets *bullet_mgr, const Ship *ship) {
  if (bullet_mgr->count < BULLET_COUNT) {
    Bullet *b = &bullet_mgr->bullets[bullet_mgr->count];
    create_bullet(b, ship, BULLET_LIFETIME);
    bullet_mgr->count++;
  }
}

void update_bullet(Bullets *bullet_mgr) {
    for (int i = 0; i < bullet_mgr->count; i++) {
      Bullet *b = &bullet_mgr->bullets[i];
      b->lifetime--;
      if (bullet_mgr->bullets[i].lifetime <= 0) {
        bullet_mgr->bullets[i] = bullet_mgr->bullets[bullet_mgr->count - 1];
        bullet_mgr->count--;
      }
      else {
        polygon_translate(&b->shape, b->velocity);
      }
  }
}
