//
// Created by redonxharja on 9/11/24.
//
#include "entities.h"

#include <time.h>

#include "config.h"

void create_bullet(Bullet *b, const Ship *ship, const double lifetime) {
  const Vector2 nose = ship->body.shape.offsets[0];
  const Vector2 stop = {.x = nose.x + BULLET_SIZE, .y = nose.y };

  b->body.shape = polygon_init(2, ship->body.shape.position, (Vector2[]){nose,stop}, ship->body.shape.angle);
  b->body.velocity.x = 5 * cos(ship->body.shape.angle);
  b->body.velocity.y = 5 * sin(ship->body.shape.angle);
  b->lifetime = lifetime;
}

double random_float() {
  return (double)rand() / (double)RAND_MAX;
}

Vector2 random_point(const double r, const double radian) {
  const double radius = r * random_float();
  return (Vector2) { .x = radius * cos(radian), .y = radius * sin(radian) };
}

Asteroid create_asteroid(const double r) {
  Asteroid asteroid = {0};
  asteroid.radius = r;
  const Vector2 position = { .x = rand() % WINDOW_WIDTH, .y = rand() % WINDOW_WIDTH };
  Vector2 points[16] = {
    random_point(r, 0),
    random_point(r, M_PI/6),
    random_point(r, M_PI/4),
    random_point(r, M_PI/3),
    random_point(r, M_PI_2),
    random_point(r, M_PI_2 + M_PI / 6),
    random_point(r, M_PI_2 + M_PI / 4),
    random_point(r, M_PI_2 + M_PI / 3),
    random_point(r, M_PI),
    random_point(r, M_PI + M_PI / 6),
    random_point(r, M_PI + M_PI / 4),
    random_point(r, M_PI + M_PI / 3),
    random_point(r, 3 * M_PI_2),
    random_point(r, 3 * M_PI_2 + M_PI / 6),
    random_point(r, 3 * M_PI_2 + M_PI / 4),
    random_point(r, 3 * M_PI_2 + M_PI / 3),
  };
  asteroid.body = create_body(polygon_init(16, position, points, 0), (Vector2){.x = random_float() / 10, .y = random_float() / 10});
  return asteroid;
}

Asteroids create_asteroids() {
  Asteroids asteroids = {0};
  asteroids.count = 8;
  for (int i = 0; i < asteroids.count; i++) {
    const Asteroid asteroid = create_asteroid(100 * random_float());
    asteroids.asteroids[i] = asteroid;
  }
  return asteroids;
}

Ship init_ship(void) {
  Ship ship = {0};

  const Vector2 position = { .x = 300, .y = 470 };
  const Vector2 p1 = { .x = 70,  .y = 0 };    // nose — now +X
  const Vector2 p2 = { .x = -30, .y = -50 };
  const Vector2 p3 = { .x = -5,  .y = -25 };
  const Vector2 p4 = { .x = -5,  .y = 25 };
  const Vector2 p5 = { .x = -30, .y = 50 };

  ship.body.shape = polygon_init(5, position, (Vector2[]){p1,p2,p3,p4,p5}, -M_PI / 2 );
  ship.body.velocity = (Vector2){ .x = 0, .y = 0 };
  ship.acceleration = (Vector2){ .x = 0, .y = 0 };

  return ship;
}

void move_ship(Ship *ship) {
  ship->body.velocity = vector_add(ship->body.velocity, ship->acceleration);
  move_body(&ship->body);
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
        move_body(&bullet_mgr->bullets[i].body);
      }
  }
}
