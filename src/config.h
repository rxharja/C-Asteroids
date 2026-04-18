//
// Created by redonxharja on 9/10/24.
//

#ifndef CONFIG_H
#define CONFIG_H

//render
#define WINDOW_WIDTH 1200

// physics
#define DRAG_FACTOR .2
#define EPSILON 0.02
#define ACCELERATION_MAGNITUDE 3000
#define ANGLE_MAGNITUDE 7

// entities
#define BULLET_COUNT 16
#define BULLET_COOLDOWN 250
#define BULLET_LIFETIME 1
#define BULLET_SIZE 10
#define BULLET_SPEED 1000
#define BULLET_DRAG 1

#define ASTEROID_COUNT 8
#define ASTEROID_ANGULAR_VELOCITY .2
#define ASTEROID_VELOCITY_FACTOR .01
#define ASTEROID_DRAG 1

#endif //CONFIG_H
