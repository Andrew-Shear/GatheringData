#include <Arduino.h>
#include <SensirionI2cScd4x.h>
#include <Wire.h>
#include <SD.h>

// macro definitions
#define NO_ERROR 0

SensirionI2cScd4x sensor;
File dataFile;

static char errorMessage[64];
static int16_t error;

void setup() {

    Serial.begin(9600);
    while (!Serial) {
        delay(100);
    }
    Wire.begin();
    sensor.begin(Wire, SCD41_I2C_ADDR_62);

    uint64_t serialNumber = 0;
    delay(30);
    // Ensure sensor is in clean state
    error = sensor.wakeUp();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute wakeUp(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
    }
    error = sensor.stopPeriodicMeasurement();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
    }
    error = sensor.reinit();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute reinit(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
    }
}

void loop() {
    // sleep for 50 seconds
    delay(50000);

    uint16_t co2Concentration = 0;
    float temperature = 0.0;
    float relativeHumidity = 0.0;
    //
    // Wake the sensor up from sleep mode.
    //
    error = sensor.wakeUp();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute wakeUp(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    //
    // Ignore first measurement after wake up.
    //
    error = sensor.measureSingleShot();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute measureSingleShot(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    //
    // Perform single shot measurement and read data.
    //
    error = sensor.measureAndReadSingleShot(co2Concentration, temperature, relativeHumidity);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute measureAndReadSingleShot(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    // Print results in physical units.
    // Serial.print("CO2 concentration [ppm]: ");
    Serial.println(co2Concentration);
}