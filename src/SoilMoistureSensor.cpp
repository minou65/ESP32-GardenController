#include "SoilMoistureSensor.h"

#define MEASUREMENT_INTERVAL_MS (12UL * 60UL * 60UL * 1000UL) // 12 Stunden in Millisekunden
#define POWER_ON_DELAY_MS 1000  // 1 Sekunde warten bis Sensor stabil ist

SoilMoistureSensor::SoilMoistureSensor(const char* id, const char* label)
    : _id(id),
    ParameterGroup(id, label),
    _lastRawValue(-1),
    _lastMeasurementTime(0) {

    snprintf(_NameValue, STRING_LEN, "Soil Sensor");
    snprintf(_MinValue, NUMBER_LEN, "0");
    snprintf(_MaxValue, NUMBER_LEN, "4095");
    snprintf(_ThresholdValue, NUMBER_LEN, "50");
    snprintf(_EnabledValue, NUMBER_LEN, "0");

    snprintf(_NameId, STRING_LEN, "%s-name", _id.c_str());
    snprintf(_MinId, STRING_LEN, "%s-min", _id.c_str());
    snprintf(_MaxId, STRING_LEN, "%s-max", _id.c_str());
    snprintf(_ThresholdId, STRING_LEN, "%s-threshold", _id.c_str());
    snprintf(_EnabledId, STRING_LEN, "%s-enabled", _id.c_str());

    this->addItem(&this->_NameParam);
    this->addItem(&this->_MinParam);
    this->addItem(&this->_MaxParam);
    this->addItem(&this->_ThresholdParam);
    this->addItem(&this->_EnabledParam);
}

void SoilMoistureSensor::applyDefaultValue() {
    _NameParam.applyDefaultValue();
    _MinParam.applyDefaultValue();
    _MaxParam.applyDefaultValue();
    _ThresholdParam.applyDefaultValue();
    _EnabledParam.applyDefaultValue();
}

void SoilMoistureSensor::begin() {
    pinMode(SOIL_MOISTURE_POWER_PIN, OUTPUT);
    digitalWrite(SOIL_MOISTURE_POWER_PIN, LOW); // Sensor ausgeschaltet
    pinMode(SOIL_MOISTURE_PIN, INPUT);

    Serial.println("Soil moisture sensor initialized");
}

const char* SoilMoistureSensor::getName() const {
    return _NameValue;
}

int SoilMoistureSensor::getMinValue() const {
    return atoi(_MinValue);
}

int SoilMoistureSensor::getMaxValue() const {
    return atoi(_MaxValue);
}

int SoilMoistureSensor::getThreshold() const {
    return atoi(_ThresholdValue);
}

bool SoilMoistureSensor::isEnabled() const {
    return strcmp(_EnabledValue, "selected") == 0;
}

int SoilMoistureSensor::measureMoisture() {
    if (!isEnabled()) {
        Serial.println("Soil sensor is disabled");
        return -1;
    }

    Serial.println("Starting soil moisture measurement...");

    // Sensor einschalten
    digitalWrite(SOIL_MOISTURE_POWER_PIN, HIGH);
    Serial.println("Sensor powered ON");

    // Warten bis Sensor stabil ist
    delay(POWER_ON_DELAY_MS);

    // Mehrere Messungen für Durchschnittswert
    long sum = 0;
    const int numReadings = 10;
    for (int i = 0; i < numReadings; i++) {
        sum += analogRead(SOIL_MOISTURE_PIN);
        delay(10);
    }
    int rawValue = sum / numReadings;

    // Sensor ausschalten
    digitalWrite(SOIL_MOISTURE_POWER_PIN, LOW);
    Serial.println("Sensor powered OFF");

    _lastRawValue = rawValue;
    _lastMeasurementTime = millis();

    Serial.printf("Soil moisture measured: %d (raw), %d%%\n", rawValue, getMoisturePercent());

    return rawValue;
}

int SoilMoistureSensor::getLastMoisture() const {
    return _lastRawValue;
}

int SoilMoistureSensor::getMoisturePercent() const {
    if (_lastRawValue < 0) return -1;

    int minVal = getMinValue();
    int maxVal = getMaxValue();

    // Map von min/max auf 0-100%
    int percent = map(_lastRawValue, minVal, maxVal, 0, 100);
    return constrain(percent, 0, 100);
}

unsigned long SoilMoistureSensor::getLastMeasurementTime() const {
    return _lastMeasurementTime;
}

bool SoilMoistureSensor::shouldMeasure() const {
    if (!isEnabled()) return false;
    if (_lastMeasurementTime == 0) return true; // Noch nie gemessen

    unsigned long elapsed = millis() - _lastMeasurementTime;
    return elapsed >= MEASUREMENT_INTERVAL_MS;
}

// Globale Instanz
SoilMoistureSensor soilMoistureSensor("soilsensor", "Soil Moisture Sensor");