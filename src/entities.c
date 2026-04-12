//
// Created by redonxharja on 9/11/24.
//
#include "entities.h"

void create_bullet(Bullet *b, const Ship *ship, const double lifetime) {
  const vector2 centroid = polygon_centroid(&ship->shape);
  const vector2 start = {.x = centroid.x, .y = centroid.y - 0 };
  const vector2 stop = {.x = centroid.x, .y = centroid.y - 10 };
  b->shape = polygon_init(2, (vector2[]){start,stop});
  // polygon_rotate(&b->shape, M_PI / 2);
  // polygon_rotate_around(centroid, &b->shape, ship->angle);
  b->velocity.y = 5 * ship->velocity.y;
  b->velocity.x = 5 * cos(ship->angle); // Example velocity based on ship's angle
  b->velocity.y = 5 * sin(ship->angle);
  b->lifetime = lifetime;
}

Ship init_ship(void) {
  Ship ship = {0};

  const vector2 p1 = { .x = 300, .y = 400 };
  const vector2 p2 = { .x = 250, .y = 500 };
  const vector2 p3 = { .x = 275, .y = 475 };
  const vector2 p4 = { .x = 325, .y = 475 };
  const vector2 p5 = { .x = 350, .y = 500 };
  const polygon polygon = polygon_init(5, (vector2[]){p1,p2,p3,p4,p5});

  ship.shape = polygon;
  ship.velocity = (vector2){ .x = 0, .y = 0 };
  ship.acceleration = (vector2){ .x = 0, .y = 0 };
  ship.angle = -M_PI / 2;
  return ship;
}

void move_ship(Ship *ship) {
  vector_add(&ship->velocity, &ship->acceleration);
  polygon_translate(&ship->shape, &ship->velocity);
}

void fire_bullet(Bullets *bullet_mgr, const Ship *ship) {
  if (bullet_mgr->count < 16) {
    Bullet *b = &bullet_mgr->bullets[bullet_mgr->count];
    create_bullet(b, ship, 100);

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
        polygon_translate(&b->shape, &b->velocity);
      }
  }
}
