//
// Created by redonxharja on 9/11/24.
//
#include "entities.h"
#include "config.h"
#include "random.h"

void create_bullet(Bullet *b, const Ship *ship, const double lifetime) {
  const Body ship_body = ship->entity.body;
  const Vector2 nose = ship_body.shape.offsets[0];
  const Vector2 stop = {.x = nose.x + BULLET_SIZE, .y = nose.y };
  const Polygon shape = polygon_init(2, (Vector2[]){nose, stop});
  const Vector2 velocity = {
    .x = BULLET_SPEED * cos(ship_body.angle),
    .y = BULLET_SPEED * sin(ship_body.angle)
  };

  b->entity.body = create_body(shape, ship_body.position, velocity, ship_body.angle, 0, 1);
  b->entity.valid = 1;
  b->lifetime = lifetime;
}

Asteroid create_asteroid(const double r) {
  Asteroid asteroid = {0};
  asteroid.radius = r;
  const Vector2 position = { .x = rand() % WINDOW_WIDTH, .y = rand() % WINDOW_WIDTH };
  Vector2 points[16] = {
    random_vector(r, 0),
    random_vector(r, M_PI/6),
    random_vector(r, M_PI/4),
    random_vector(r, M_PI/3),
    random_vector(r, M_PI_2),
    random_vector(r, M_PI_2 + M_PI / 6),
    random_vector(r, M_PI_2 + M_PI / 4),
    random_vector(r, M_PI_2 + M_PI / 3),
    random_vector(r, M_PI),
    random_vector(r, M_PI + M_PI / 6),
    random_vector(r, M_PI + M_PI / 4),
    random_vector(r, M_PI + M_PI / 3),
    random_vector(r, 3 * M_PI_2),
    random_vector(r, 3 * M_PI_2 + M_PI / 6),
    random_vector(r, 3 * M_PI_2 + M_PI / 4),
    random_vector(r, 3 * M_PI_2 + M_PI / 3),
  };

  asteroid.entity.body = create_body(
    polygon_init(16, points),
    position,
    (Vector2){.x = random_range(-1, 1) / 2., .y = random_range(-1, 1) / 2.},
    -M_PI_2,
    random_int_choice(-1, 1) * .005,
    1
  );

  asteroid.entity.valid = 1;

  return asteroid;
}

void create_asteroids(Asteroids *asteroids) {
  for (int i = 0; i < asteroids->count; i++) {
    const Asteroid asteroid = create_asteroid(100 * random_float());
    asteroids->asteroids[i] = asteroid;
  }
}

Ship init_ship(void) {
  Ship ship = {0};

  const Vector2 position = { .x = WINDOW_WIDTH / 2, .y = WINDOW_WIDTH / 2 };
  const Vector2 p1 = { .x = 70,  .y = 0 };    // nose — now +X
  const Vector2 p2 = { .x = -30, .y = -50 };
  const Vector2 p3 = { .x = -5,  .y = -25 };
  const Vector2 p4 = { .x = -5,  .y = 25 };
  const Vector2 p5 = { .x = -30, .y = 50 };

  const Polygon shape = polygon_init(5, (Vector2[]){p1,p2,p3,p4,p5});
  const Vector2 velocity = (Vector2){ .x = 0, .y = 0 };

  ship.entity.body = create_body(shape, position, velocity, -M_PI_2, 0, DRAG_FACTOR);
  ship.entity.valid = 1;
  body_integrate(&ship.entity.body, 1);
  return ship;
}

void fire_bullet(Bullets *bullet_mgr, const Ship *ship) {
  for (int i = 0; i < BULLET_COUNT; i++) {
    Bullet *b = &bullet_mgr->bullets[i];
    if (b->entity.valid) continue;
    create_bullet(b, ship, BULLET_LIFETIME);
    break;
  }
}

void degrade_bullet(Bullet *b) {
    if (!b->entity.valid) return;
    b->lifetime--;
    if (b->lifetime <= 0) b->entity.valid = 0;
}