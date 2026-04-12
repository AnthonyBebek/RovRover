#include <Arduino.h>
#include "../include/Roverconfig.h"
#include "../include/motorControl.h"
#include "../include/encoderControl.h"


float target_left_speed = 0;
float target_right_speed = 0;

targetVelocities velocities = {0, 0}; // Initialize target velocities to zero

int computePID(PID &pid, float target_vel, float measured_vel, float dt, int min_pwm) {

    float error = target_vel - measured_vel;
    float derivative = (error - pid.last_error) / dt;
    float integral_candidate = pid.integral + error * dt;
    float output = pid.Kp * error +
                   pid.Ki * integral_candidate +
                   pid.Kd * derivative;

    float feedforward = FEEDFORWARD_GAIN * target_vel; // Feedforward term to help reach the target speed faster
    output += feedforward;

    if (fabs(target_vel) < 0.001) {
        pid.integral = 0;
        pid.last_error = 0;
        return 0;
    }

    if (output > 0) output += min_pwm;
    else if (output < 0) output -= min_pwm;

    if (output > MAX_PWM) output = MAX_PWM;
    if (output < -MAX_PWM) output = -MAX_PWM;

    // anti-windup: only accept integral if NOT saturated
    if (output < MAX_PWM && output > -MAX_PWM) {
        pid.integral = integral_candidate;
    }

    pid.last_error = error;
    return (int)output;
}

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

void updateLeftMotorSpeed(PID &pid, int delta_ticks, float dt, float speed){
    float measured_vel = getWheelVelocity(delta_ticks, dt);
    int pwm = computePID(pid, speed, measured_vel, dt, MIN_PWM_LEFT);
    setMotor(LF_IN, LR_IN, pwm);
}

void updateRightMotorSpeed(PID &pid, int delta_ticks, float dt, float speed){
    float measured_vel = getWheelVelocity(delta_ticks, dt);
    int pwm = computePID(pid, speed, measured_vel, dt, MIN_PWM_RIGHT);
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

    target_left_speed = v - w * (WHEEL_BASE / 2);
    target_right_speed = v + w * (WHEEL_BASE / 2);

    target_left_speed = constrain(target_left_speed, -MAX_LINEAR_VELOCITY, MAX_LINEAR_VELOCITY);
    target_right_speed = constrain(target_right_speed, -MAX_LINEAR_VELOCITY, MAX_LINEAR_VELOCITY);

    velocities.left = target_left_speed;
    velocities.right = target_right_speed;

    return true;
}