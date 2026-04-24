//
// Created by redonxharja on 9/11/24.
//
#include "entities.h"
#include "config.h"
#include "polygon.h"
#include "random.h"

void create_bullet(Bullet *b, const Ship *ship, const double lifetime) {
  destroy_body(&b->entity.body);
  const Body ship_body = ship->entity.body;
  const Vector2 nose = ship_body.shape.offsets[0];
  const Vector2 stop = {.x = nose.x + BULLET_SIZE, .y = nose.y };
  const Polygon shape = polygon_init(2, (Vector2[]){nose, stop});
  const Vector2 velocity = {
    .x = BULLET_SPEED * cos(ship_body.angle),
    .y = BULLET_SPEED * sin(ship_body.angle)
  };

  b->entity.body = create_body(shape, ship_body.position, velocity, ship_body.angle, 0, BULLET_DRAG, 1);
  b->entity.valid = 1;
  b->lifetime = lifetime;
}

Asteroid create_asteroid(const double r) {
  Asteroid asteroid = {0};
  asteroid.radius = r;
  const Vector2 position = { .x = rand() % WINDOW_WIDTH, .y = rand() % WINDOW_HEIGHT };

  const double angle_inc =  2 * M_PI / ASTEROID_POLYGON_COUNT;

  Vector2 points[ASTEROID_POLYGON_COUNT] = {};
  double current_angle = 0;
  for (int i = 0; i < ASTEROID_POLYGON_COUNT; i++) {
    points[i] = random_vector(r, current_angle);
    current_angle += angle_inc;
  }

  asteroid.entity.body = create_body(
    polygon_init(ASTEROID_POLYGON_COUNT, points),
    position,
    (Vector2) {
      .x = random_range(-ASTEROID_VELOCITY_FACTOR, ASTEROID_VELOCITY_FACTOR),
      .y = random_range(-ASTEROID_VELOCITY_FACTOR, ASTEROID_VELOCITY_FACTOR),
    },
    -M_PI_2, random_int_choice(-1, 1) * ASTEROID_ANGULAR_VELOCITY,
    ASTEROID_DRAG, r
  );

  asteroid.entity.valid = 1;

  return asteroid;
}

void create_asteroids(Asteroids *asteroids, const int count) {
  for (int i = 0; i < count; i++) {
    const Asteroid asteroid = create_asteroid(ASTEROID_RADIUS);
    asteroids->asteroids[i] = asteroid;
  }
}

void try_collide_asteroids(Asteroid *a, Asteroid *b) {
  if (!a->entity.valid || !b->entity.valid) return;
  if (!is_colliding_sat(&a->entity.body, &b->entity.body)) return;
  collision_elastic(&a->entity.body, &b->entity.body);
}

void check_asteroids_collision(Asteroids *asteroids) {
  for (int i = 0; i < ASTEROID_COUNT_MAX - 1; i++) {
    for (int j = i + 1; j < ASTEROID_COUNT_MAX; j++) {
      Asteroid *a = &asteroids->asteroids[i];
      Asteroid *b = &asteroids->asteroids[j];
      try_collide_asteroids(a, b);
    }
  }
}

int split_asteroid(Asteroids *asteroids, const int a_idx) {
  int score = 0;
  Asteroid *a = &asteroids->asteroids[a_idx];
  const Vector2 pos = a->entity.body.position;
  const double r = a->radius;
  a->entity.valid = 0;

  if (r > ASTEROID_LARGE_THRESHOLD) score = 100;
  else if (r > ASTEROID_MEDIUM_THRESHOLD) score = 50;
  else return 250;

  int added = 0;
  for (int i = 0; i < ASTEROID_COUNT_MAX; i++) {
    if (added >= 2) break;
    if (asteroids->asteroids[i].entity.valid) continue;
    asteroids->asteroids[i] = create_asteroid(r / 2.);
    asteroids->asteroids[i].entity.body.position = pos;
    added++;
  }

  return score;
}

int try_collide_bullet(Bullet *b, Asteroids *asteroids, const int a_idx) {
  const Asteroid *a = &asteroids->asteroids[a_idx];
  if (!a->entity.valid || !b->entity.valid) return 0;
  if (!is_colliding_sat(&b->entity.body,&a->entity.body)) return 0;
  b->entity.valid = 0;
  return split_asteroid(asteroids, a_idx);
}

int check_bullet_collision(Bullets *bullets, Asteroids *asteroids, Explosions *explosions) {
  int score = 0;
  for (int i = 0; i < BULLET_COUNT; i++) {
    Bullet *b = &bullets->bullets[i];
    for (int j = 0; j < ASTEROID_COUNT_MAX; j++) {
      const int points = try_collide_bullet(b, asteroids, j);
      if (points > 0) create_particles(explosions, asteroids->asteroids[j].entity.body.position);
      score += points;
    }
  }
  return score;
}

int asteroids_cleared(const Asteroids *asteroids) {
  for (int i = 0; i < ASTEROID_COUNT_MAX; i++) {
    if (asteroids->asteroids[i].entity.valid) return 0;
  }
  return 1;
}

Particle init_particles(const double lifetime) {
  Vector2 point[1] = { { 1, 1 }};
  Particle explosion = {
    (Entity) {
      create_body( polygon_init(1, point), (Vector2){ 0, 0 },
        (Vector2) {
            .x = random_range(-75, 75),
            .y = random_range(-75, 75)
          }, random_range(0, 4), 0, 1, 0), 0
    }, lifetime
  };

  body_integrate(&explosion.entity.body, 1);
  return explosion;
}

void create_particles(Explosions *explosions, const Vector2 position) {
  int valid_particles = 0;
  for (int i = 0; i < TOTAL_PARTICLES; i++) {
    if (valid_particles >= PARTICLE_COUNT) return;
    Particle *particle = &explosions->particles[i];
    if (particle->entity.valid) continue;
    body_set_position(&particle->entity.body, position);
    particle->entity.valid = 1;
    particle->lifetime = PARTICLE_LIFETIME;
    valid_particles++;
  }
}

void init_explosions(Explosions *explosions) {
  for (int i = 0; i < TOTAL_PARTICLES; i++) {
    explosions->particles[i] = init_particles(PARTICLE_LIFETIME);
  }
}

void free_explosions(const Explosions *explosions) {
  for (int i = 0; i < TOTAL_PARTICLES; i++) {
    destroy_body(&explosions->particles[i].entity.body);
  }
}

Ship init_ship(void) {
  Ship ship = {0};
  ship.lives = LIVES;

  const Vector2 position = { .x = WINDOW_WIDTH / 2., .y = WINDOW_HEIGHT / 2. };
  const Vector2 p1 = { .x = 70,  .y = 0 };    // nose — now +X
  const Vector2 p2 = { .x = -30, .y = -50 };
  const Vector2 p3 = { .x = -5,  .y = -25 };
  const Vector2 p4 = { .x = -5,  .y = 25 };
  const Vector2 p5 = { .x = -30, .y = 50 };

  const Polygon shape = polygon_init(5, (Vector2[]){p1,p2,p3,p4,p5});
  const Vector2 velocity = (Vector2){ .x = 0, .y = 0 };

  ship.entity.body = create_body(shape, position, velocity, -M_PI_2, 0, DRAG_FACTOR, 1);
  ship.entity.valid = 1;
  body_integrate(&ship.entity.body, 1);
  return ship;
}

int is_ship_colliding(const Ship *ship, const Asteroids *asteroids) {
  for (int i = 0; i < ASTEROID_COUNT_MAX; i++) {
    const Asteroid *a = &asteroids->asteroids[i];
    if (!a->entity.valid) continue;
    if (is_colliding_sat(&ship->entity.body, &a->entity.body)) return 1;
  }
  return 0;
}

void fire_bullet(Bullets *bullet_mgr, const Ship *ship) {
  for (int i = 0; i < BULLET_COUNT; i++) {
    Bullet *b = &bullet_mgr->bullets[i];
    if (b->entity.valid) continue;
    create_bullet(b, ship, BULLET_LIFETIME);
    break;
  }
}

void degrade_bullet(Bullet *b, const double dt) {
    if (!b->entity.valid) return;
    b->lifetime -= dt;
    if (b->lifetime <= 0) b->entity.valid = 0;
}

void destroy_bullets(const Bullets *bullets) {
  for (int i = 0; i < BULLET_COUNT; i++) {
    destroy_body(&bullets->bullets[i].entity.body);
  }
}

void destroy_asteroids(const Asteroids *asteroids) {
  for (int i = 0; i < ASTEROID_COUNT_MAX; i++) {
    destroy_body(&asteroids->asteroids[i].entity.body);
  }
}
