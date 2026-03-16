#include <Arduino.h>
#include <Wire.h>
#include "RoverConfig.h"
#include "../include/encoderControl.h"
#include "../include/motorControl.h"
#include "../include/roverCalibrate.h"

#define CMD_BUFFER 64

float lastUpdate = millis();

char commandBuffer[CMD_BUFFER];
int commandIndex = 0;
unsigned long last_pid_update = 0;


void handleCommand(char* cmd) {

    if (strncmp(cmd, "ENC", 3) == 0) {
        readEncoderTicks();
    }
    else if (strncmp(cmd, "ANGLE", 5) == 0) {

        debugEncoderAngles();

    }
    else if (strncmp(cmd, "DIST", 4) == 0) {

        getEncoderDistances();

    }
    else if (strncmp(cmd, "VEL", 3) == 0) {

        handleMotorCommand(cmd);

    }
    else if (strncmp(cmd, "CAL", 3) == 0) {

        Serial.println("Calibrated speed LEFT: " + String(CalibrateSpeeds(encoderA)));
        Serial.println("Calibrated speed RIGHT: " + String(CalibrateSpeeds(encoderB)));

    }
}

void readSerial() {

    while (Serial.available()) {

        char c = Serial.read();

        if (c == '\n') {           // command finished
            commandBuffer[commandIndex] = '\0';

            handleCommand(commandBuffer);

            commandIndex = 0;      // reset buffer
        }
        else {

            if (commandIndex < CMD_BUFFER - 1) {
                commandBuffer[commandIndex++] = c;
            }

        }
    }
}


void setup() {
    Serial.begin(115200);
    delay(1000); // Wait for serial to initialize
    Serial.println("RovRover Starting...");
    
    initMotors();
    initEncoders();

    //Serial.println("Calibrated speed LEFT: " + String(CalibrateSpeeds(encoderA)));
    //Serial.println("Calibrated speed RIGHT: " + String(CalibrateSpeeds(encoderB)));
    Serial.println("Ready!");
    stopMotors(); // Ensure motors are stopped after calibration
    delay(10000); // Wait for 10 seconds to allow time for the user to open the serial monitor
}

void loop() {
    // Get all possible data from encoder
    readSerial();

    if (millis() - lastUpdate > 1000) { // Update every 1000 ms (1 Hz)
        //print seperator to make it easier to see
        //Serial.println("------------------------------------");
        //updateEncoders();
        //getEncoderDistances();
        //readEncoderTicks();
        //debugEncoderAngles();
        lastUpdate = millis();
    }

    float dt = (millis() - last_pid_update) / 1000.0; // Convert to seconds
    if (dt >= 0.1) { // Update PID every 100 ms

        int delta_left = getEncoderDeltaTicks(encoderA, last_left_angle);
        int delta_right = getEncoderDeltaTicks(encoderB, last_right_angle);
        updateLeftMotorSpeed(leftPID, delta_left, dt, velocities.left);
        updateRightMotorSpeed(rightPID, delta_right, dt, velocities.right);
        last_pid_update = millis();
        Serial.println("------------------------------");
    }
}
