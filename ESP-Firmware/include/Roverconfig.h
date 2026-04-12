// RoverConfig.h - Centralized configuration for the rover
#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H

// Pin definitions for encoders and motors

#define SDA1 39 // I2C bus 1 (Encoder A)
#define SCL1 37 // I2C bus 1 (Encoder A)
#define SDA3 18 // I2C bus 3 (Encoder B)
#define SCL3 16 // I2C bus 3 (Encoder B)
#define SDA2 35 // I2C bus 2 (Encoder B)
#define SCL2 33 // I2C bus 2 (Encoder B)



#define RF_IN 11 // Driver 1 (R Forward)
#define RR_IN 12 // Driver 1 (R Reverse)
#define LF_IN 7 // Driver 2 (L Forward)
#define LR_IN 9 // Driver 2 (L Reverse)

// Set encoder parameters
#define LEFT_DIRECTION -1 // 1 for normal, -1 for reversed
#define RIGHT_DIRECTION 1 // 1 for normal, -1 for reversed

// Rover parameters
#define WHEEL_RADIUS 0.03 // Radius of the wheels in meters (30 mm)
#define TICKS_PER_REVOLUTION 4096 // Number of ticks per full revolution of the encoder
#define WHEEL_BASE 0.135 // Distance between the two wheels in meters (135 mm)

#define MAX_LINEAR_VELOCITY 0.3 // Maximum linear velocity in m/s
#define MAX_ANGULAR_VELOCITY 3.14/2 // Maximum angular velocity in rad/s (180 degrees/s)

#define MIN_PWM_LEFT 70 // Minimum PWM value for motor control (Printed in calibration)
#define MIN_PWM_RIGHT 70 // Minimum PWM value for motor control (Printed in calibration)

#define MAX_PWM 255 // Maximum PWM value for motor control


// PID controller parameters
#define LEFT_KP 1200.0 // Proportional gain
#define LEFT_KI 50.0// Integral gain
#define LEFT_KD 0.0 // Derivative gain

#define RIGHT_KP 1200.0 // Proportional gain
#define RIGHT_KI 50.0// Integral gain
#define RIGHT_KD 0.0 // Derivative gain

#define FEEDFORWARD_GAIN 700.0 // Feedforward gain for motor control


#endif