#include <Arduino.h>
#include "RoverConfig.h"
#include "encoderControl.h"

// Define different buses
TwoWire WireA(0); // I2C bus 1
TwoWire WireB(1); // I2C bus 2

// Define encoders on each bus
AS5600 encoderA(&WireA);
AS5600 encoderB(&WireB);

long left_ticks = 0;
long right_ticks = 0;

int last_left_angle = 0;
int last_right_angle = 0;

const float WHEEL_CIRCUMFERENCE = 2 * PI * WHEEL_RADIUS; // Circumference of the wheel in meters
const float DISTANCE_PER_TICK = WHEEL_CIRCUMFERENCE / TICKS_PER_REVOLUTION; // Distance traveled per tick in meters

void initEncoders() {
    WireA.begin(SDA1, SCL1);
    WireB.begin(SDA2, SCL2);
    if (!encoderA.begin()) {
        Serial.println("Failed to initialize encoder A");
    }
    if (!encoderB.begin()) {
        Serial.println("Failed to initialize encoder B");
    }
}

bool scanI2CWireA() {
    Serial.println("Scanning I2C bus...");
    for (uint8_t address = 1; address < 127; address++) {
        WireA.beginTransmission(address);
        if (WireA.endTransmission() == 0) {
            Serial.print("I2C device on A found at address 0x");
            if (address < 16) Serial.print("0");
            return true;
        }
    }
    return false;
}

bool scanI2CWireB() {
    Serial.println("Scanning I2C bus...");
    for (uint8_t address = 1; address < 127; address++) {
        WireB.beginTransmission(address);
        if (WireB.endTransmission() == 0) {
            Serial.print("I2C device on B found at address 0x");
            if (address < 16) Serial.print("0");
            return true;
        }
    }
return false;
}

void updateEncoder(AS5600 &encoder, int &lastAngle, long &ticks) {
    if (!encoder.isConnected()) {
        return; // Skip if encoder is not connected
    }

    int current_angle = encoder.readAngle();
    int delta = current_angle - lastAngle;
    if (delta > 2048) {
        delta -= 4096; // Handle wrap-around
    } else if (delta < -2048) {
        delta += 4096; // Handle wrap-around
    }
    ticks += delta;
    lastAngle = current_angle;
}

void updateEncoders() {
    updateEncoder(encoderA, last_left_angle, left_ticks);
    updateEncoder(encoderB, last_right_angle, right_ticks);
}

void getEncoderTicks() {

    // To avoid partial updates, we read both encoders and then print the results together
    long l = left_ticks;
    long r = right_ticks;

    Serial.print("ENC ");
    Serial.print(l);
    Serial.print(' ');
    Serial.print(r);
    Serial.print(' ');
    Serial.println(millis());
}

void debugEncoderAngles(){

    // To avoid partial updates, we read both encoders and then print the results together
    long l = encoderA.readAngle();
    long r = encoderB.readAngle();

    Serial.print("ANGLE ");
    Serial.print(l);
    Serial.print(' ');
    Serial.print(r);
    Serial.print(' ');
    Serial.println(millis());
}

void resetEncoders() {
    left_ticks = 0;
    right_ticks = 0;
    last_left_angle = encoderA.readAngle();
    last_right_angle = encoderB.readAngle();
}

void getEncoderDistances() {
    // To avoid partial updates, we read both encoders and then print the results together
    long l = left_ticks;
    long r = right_ticks;

    float left_distance = l * DISTANCE_PER_TICK;
    float right_distance = r * DISTANCE_PER_TICK;

    Serial.print("DIST ");
    Serial.print(left_distance, 4);
    Serial.print(' ');
    Serial.print(right_distance, 4);
    Serial.print(' ');
    Serial.println(millis());
}