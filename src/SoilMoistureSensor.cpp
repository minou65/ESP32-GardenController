#include "SoilMoistureSensor.h"

#define MEASUREMENT_INTERVAL_MS (12UL * 60UL * 60UL * 1000UL)
#define POWER_ON_DELAY_MS 1000

SoilMoistureSensor::SoilMoistureSensor(const char* id, const char* label, int analogPin, int powerPin)
    : _id(id),
    ParameterGroup(id, label),
    _analogPin(analogPin),
    _powerPin(powerPin),
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
    pinMode(_powerPin, OUTPUT);
    digitalWrite(_powerPin, LOW);
    pinMode(_analogPin, INPUT);

    Serial.printf("Soil moisture sensor '%s' initialized (Pin: %d, Power: %d)\n", 
        _NameValue, _analogPin, _powerPin);
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
        Serial.println("Sensor is disabled, skipping measurement");
        return -1;
    }

    digitalWrite(_powerPin, HIGH);
    delay(POWER_ON_DELAY_MS);

    int rawValue_ = analogRead(_analogPin);

    digitalWrite(_powerPin, LOW);

    _lastRawValue = rawValue_;
    _lastMeasurementTime = millis();

    Serial.printf("Soil moisture measured: %d (raw)\n", rawValue_);
    return rawValue_;
}

int SoilMoistureSensor::getLastMoisture() const {
    return _lastRawValue;
}

int SoilMoistureSensor::getMoisturePercent() const {
    if (_lastRawValue < 0) return -1;

    int min_ = getMinValue();
    int max_ = getMaxValue();

    if (max_ <= min_) return 0;

    int percent_ = map(_lastRawValue, min_, max_, 0, 100);
    return constrain(percent_, 0, 100);
}

unsigned long SoilMoistureSensor::getLastMeasurementTime() const {
    return _lastMeasurementTime;
}

bool SoilMoistureSensor::shouldMeasure() const {
    if (!isEnabled()) return false;
    
    if (_lastMeasurementTime == 0) return true;

    unsigned long elapsed_ = millis() - _lastMeasurementTime;
    return elapsed_ >= MEASUREMENT_INTERVAL_MS;
}

SoilMoistureSensor soilMoistureSensors[MAX_SOIL_SENSORS] = {
    SoilMoistureSensor("soilsensor1", "Soil Sensor 1", SOIL_MOISTURE_PIN_1, SOIL_MOISTURE_POWER_PIN_1),
    SoilMoistureSensor("soilsensor2", "Soil Sensor 2", SOIL_MOISTURE_PIN_2, SOIL_MOISTURE_POWER_PIN_2)
};