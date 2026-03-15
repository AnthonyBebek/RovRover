#ifndef ENCODER_CONTROL_H
#define ENCODER_CONTROL_H

#include <AS5600.h>

extern AS5600 encoderA;
extern AS5600 encoderB;

void initEncoders();
bool scanI2CWireA();
bool scanI2CWireB();
void updateEncoders();
void getEncoderTicks();
void debugEncoderAngles();
void resetEncoders();
void getEncoderDistances();


#endif