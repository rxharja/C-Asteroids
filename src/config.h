//
// Created by redonxharja on 9/10/24.
//

#ifndef CONFIG_H
#define CONFIG_H

/*--render--*/
//1280×960
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
#define MARGIN_X (WINDOW_WIDTH / 4)
#define MARGIN_Y (WINDOW_HEIGHT / 6)

/*--physics--*/
#define DRAG_FACTOR .2
#define ACCELERATION_MAGNITUDE 3000
#define ANGLE_MAGNITUDE 7

/*--game--*/
#define PAUSE_TIME 3000 // milliseconds
#define LIVES 3
#define TITLE_OFFSET 100
#define WAVE_TEXT_OFFSET 300

/*--sound--*/
#define BEAT_TIMER 1 // seconds

/*--entities-- */

// bullets
#define BULLET_COUNT 16
#define BULLET_COOLDOWN 250
#define BULLET_LIFETIME 0.5
#define BULLET_SIZE 10
#define BULLET_SPEED 1500
#define BULLET_DRAG 1

// asteroids
#define BASE_ASTEROID_SPAWN_COUNT 8
#define ASTEROID_COUNT_MAX 32
#define ASTEROID_POLYGON_COUNT 16
#define ASTEROID_ANGULAR_VELOCITY .2
#define ASTEROID_RADIUS 100
#define ASTEROID_DESTROY_RADIUS 25
#define ASTEROID_LARGE_THRESHOLD 50
#define ASTEROID_MEDIUM_THRESHOLD 25
#define ASTEROID_VELOCITY_FACTOR 200
#define ASTEROID_DRAG 1

// particles
#define PARTICLE_COUNT 16
#define TOTAL_PARTICLES (PARTICLE_COUNT * ASTEROID_COUNT_MAX)
#define PARTICLE_LIFETIME 1 // seconds
// ship
#define IFRAMES 3.0 // seconds
#endif //CONFIG_H
