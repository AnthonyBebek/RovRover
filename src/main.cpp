#include <Arduino.h>
#include <Wire.h>
#include "RoverConfig.h"
#include "encoderControl.h"
#include "motorControl.h"

#define CMD_BUFFER 64

float lastUpdate = millis();

char commandBuffer[CMD_BUFFER];
int commandIndex = 0;

void handleCommand(char* cmd) {

    if (strncmp(cmd, "ENC", 3) == 0) {

        getEncoderTicks();

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
    Serial.println("RovRover Starting...");
    
    initMotors();
    initEncoders();

    Serial.println("Ready!");
}

void loop() {
    // Get all possible data from encoder
    readSerial();

    if (millis() - lastUpdate > 33) { // Update every 33 ms (30 Hz)
        updateEncoders();
        getEncoderDistances();
        getEncoderTicks();
        lastUpdate = millis();
    }
}
