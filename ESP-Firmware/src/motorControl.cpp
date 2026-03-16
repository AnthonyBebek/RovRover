#include <Arduino.h>
#include "../include/RoverConfig.h"
#include "../include/MotorControl.h"
#include "../include/encoderControl.h"


float target_left_speed = 0;
float target_right_speed = 0;

targetVelocities velocities = {0, 0}; // Initialize target velocities to zero

int computePID(PID &pid, float target_vel, float measured_vel, float dt, int min_pwm) {
    float error = target_vel - measured_vel;
    pid.integral += error * dt;
    float derivative = (error - pid.last_error) / dt;

    float output = (pid.Kp * error + pid.Ki * pid.integral + pid.Kd * derivative) * PWM_PER_MS; // Convert velocity error to PWM output

    // First check if target vel is zero
    if (fabs(target_vel) < 0.001) {
        // If target velocity is zero, we want to stop the motor, so we set output to zero
        if (fabs(output) < min_pwm) {
            output = 0; // If the output is within the deadzone, set it to zero
        }
        else{
            // Add minimum PWM to overcome motor deadzone
            output = constrain(output, min_pwm, MAX_PWM); // Ensure output is within max limits
            if (output > 0) output += min_pwm;
            if (output < 0) output -= min_pwm;
        }
    }

    // Constrain output to max PWM limits
    if (output > MAX_PWM) output = MAX_PWM;
    if (output < -MAX_PWM) output = -MAX_PWM;

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
    Serial.println("Left Motor - Target: " + String(speed) + " m/s, Measured: " + String(measured_vel) + " m/s, PWM: " + String(pwm));
}

void updateRightMotorSpeed(PID &pid, int delta_ticks, float dt, float speed){
    float measured_vel = getWheelVelocity(delta_ticks, dt);
    int pwm = computePID(pid, speed, measured_vel, dt, MIN_PWM_RIGHT);
    Serial.println("Right Motor - Target: " + String(speed) + " m/s, Measured: " + String(measured_vel) + " m/s, PWM: " + String(pwm));
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