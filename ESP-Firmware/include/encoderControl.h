#ifndef ENCODER_CONTROL_H
#define ENCODER_CONTROL_H

#include <AS5600.h>

extern AS5600 encoderA;
extern AS5600 encoderB;

void initEncoders();
bool scanI2CWireA();
bool scanI2CWireB();
void updateEncoders();
int getEncoderTicks(AS5600 &encoder);
void readEncoderTicks();
void debugEncoderAngles();
void resetEncoders();
void getEncoderDistances();
float getWheelVelocity(int delta_ticks, float dt);
int getEncoderDeltaTicks(AS5600 &encoder, long &last_angle);

struct PID{
    float Kp;
    float Ki;
    float Kd;
    float integral;
    float last_error;

    PID(float p, float i, float d) : Kp(p), Ki(i), Kd(d), integral(0), last_error(0) {}
};

extern PID leftPID;
extern PID rightPID;

extern long last_right_angle;
extern long last_left_angle;
extern long left_ticks;
extern long right_ticks;

#endif