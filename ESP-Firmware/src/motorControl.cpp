#include <Arduino.h>
#include "../include/RoverConfig.h"
#include "../include/MotorControl.h"

int speedToPWM(float speed) {
    float normalized = speed / MAX_LINEAR_VELOCITY; // Normalize to -1.0 to 1.0
    normalized = constrain(normalized, -1.0, 1.0); // Ensure within bounds
    return normalized * MAX_PWM; // Scale to PWM range
}

void setMotor(int in1, int in2, int pwm){
    if (pwm > 0) { // Forward
        analogWrite(in1, pwm);
        analogWrite(in2, 0);
    } else if (pwm < 0) { // Backward
        analogWrite(in1, 0);
        analogWrite(in2, -pwm);
    } else { // Stop
        analogWrite(in1, 0);
        analogWrite(in2, 0);
    }
}

void setLeftMotorSpeed(float speed){
    int pwm = speedToPWM(speed);
    setMotor(LF_IN, LR_IN, pwm);
}

void setRightMotorSpeed(float speed){
    int pwm = speedToPWM(speed);
    setMotor(RF_IN, RR_IN, pwm);
}

void stopMotors(){
    setLeftMotorSpeed(0);
    setRightMotorSpeed(0);
}

void initMotors() {
    // Initialize motor control pins
    pinMode(LF_IN, OUTPUT);
    pinMode(LR_IN, OUTPUT);
    pinMode(RF_IN, OUTPUT);
    pinMode(RR_IN, OUTPUT);

    stopMotors(); // Ensure motors are stopped at startup
}

bool handleMotorCommand(char* command){
    float v = 0.0; // Linear velocity (m/s)
    float w = 0.0; // Angular velocity (rad/s)

    int parsed = sscanf(command, "VEL %f %f", &v, &w);; // Expecting format: "VEL <linear_velocity> <angular_velocity>"
    if (parsed != 2) {
        return false; // Invalid command format
    }

    float left_speed = v - w * (WHEEL_BASE / 2);
    float right_speed = v + w * (WHEEL_BASE / 2);

    setLeftMotorSpeed(left_speed);
    setRightMotorSpeed(right_speed);

    return true;
}


