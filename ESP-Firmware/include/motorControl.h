#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "encoderControl.h"

void initMotors();
bool handleMotorCommand(char* command);
void stopMotors();
void setMotor(int in1, int in2, int pwm);
void updateLeftMotorSpeed(PID &pid, int delta_ticks, float dt, float speed);
void updateRightMotorSpeed(PID &pid, int delta_ticks, float dt, float speed);

struct targetVelocities {
    float left;
    float right;
};

extern targetVelocities velocities;

#define PWM_PER_MS (MAX_PWM / MAX_LINEAR_VELOCITY) // PWM value per m/s, used for converting speed to PWM

#endif