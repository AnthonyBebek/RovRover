// RoverConfig.h - Centralized configuration for the rover
#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H

// Pin definitions for encoders and motors

#define SDA1 39 // I2C bus 1 (Encoder A)
#define SCL1 37 // I2C bus 1 (Encoder A)
#define SDA2 35 // I2C bus 2 (Encoder B)
#define SCL2 33 // I2C bus 2 (Encoder B)

#define SDA3 18 // I2C bus 3 (Unused, reserved for future sensors)
#define SCL3 16 // I2C bus 3 (Unused, reserved for future sensors)

#define LF_IN 12 // Driver 1 (Left Forward)
#define LR_IN 11 // Driver 1 (Left Reverse)
#define RF_IN 9 // Driver 2 (Right Forward)
#define RR_IN 7 // Driver 2 (Right Reverse)

// Rover parameters
#define WHEEL_RADIUS 0.03 // Radius of the wheels in meters (30 mm)
#define TICKS_PER_REVOLUTION 4096 // Number of ticks per full revolution of the encoder
#define WHEEL_BASE 0.135 // Distance between the two wheels in meters (135 mm)

#define MAX_LINEAR_VELOCITY 0.5 // Maximum linear velocity in m/s
#define MAX_ANGULAR_VELOCITY 3.14 // Maximum angular velocity in rad/s (180 degrees/s)
#define MAX_PWM 255 // Maximum PWM value for motor control

#endif