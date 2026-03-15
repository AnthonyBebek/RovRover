// RoverConfig.h - Centralized configuration for the rover
#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H

// Pin definitions for encoders and motors

#define SDA1 39 // I2C bus 1 (Encoder A)
#define SCL1 37 // I2C bus 1 (Encoder A)
#define SDA2 35 // I2C bus 2 (Encoder B)
#define SCL2 33 // I2C bus 2 (Encoder B)

#define FR_IN 12 // Driver 1
#define FL_IN 11 // Driver 1
#define BR_IN 9 // Driver 2
#define BL_IN 7 // Driver 2

// Rover parameters
#define WHEEL_RADIUS 0.03 // Radius of the wheels in meters (30 mm)
#define TICKS_PER_REVOLUTION 4096 // Number of ticks per full revolution of the encoder
#define WHEEL_BASE 0.135 // Distance between the two wheels in meters (135 mm)

#define MAX_LINEAR_VELOCITY 0.5 // Maximum linear velocity in m/s
#define MAX_ANGULAR_VELOCITY 3.14 // Maximum angular velocity in rad/s (180 degrees/s)
#define MAX_PWM 255 // Maximum PWM value for motor control

#endif