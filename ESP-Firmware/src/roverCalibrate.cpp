#include <Arduino.h>
#include "../include/RoverConfig.h"
#include "../include/roverCalibrate.h"
#include "../include/encoderControl.h"
#include "../include/motorControl.h"

int CalibrateSpeeds(AS5600 &encoder) {
    Serial.println("Starting calibration...");
    stopMotors();
    // Wait for motors to stop
    delay(1000);
    bool DetectedMovement = false;
    int i;

    for (i = 0; i <= 255; i += 1) {
        int encoderStart = encoder.readAngle();

        if (!DetectedMovement) {
            setMotor(LF_IN, LR_IN, i);
            setMotor(RF_IN, RR_IN, i);
            delay(50); // Short delay to allow the motors to respond
            DetectedMovement = (encoder.readAngle() >= encoderStart + 10) || (encoder.readAngle() <= encoderStart - 10); // Check if the encoder has moved
        }
        else {
            stopMotors();
            break; // Exit the loop once movement is detected
        }
        
    }
    return i; // Return the first speed that causes movement
}