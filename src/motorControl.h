#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

void initMotors();
bool handleMotorCommand(char* command);
void stopMotors();

#endif