#include "SoilMoistureSensor.h"
#include "EventLogger.h"

#define MEASUREMENT_INTERVAL_MS (1UL * 60UL * 60UL * 1000UL) // 1 Stunde
#define POWER_ON_DELAY_MS 1000
#define STARTUP_STABILIZATION_MS 45000UL
#define SENSOR_STAGGER_MS 1500UL

SoilMoistureSensor::SoilMoistureSensor(const char* id, const char* label, int analogPin, int powerPin)
    : _id(id),
    ParameterGroup(id, label),
    _analogPin(analogPin),
    _powerPin(powerPin),
    _lastRawValue(-1),
    _lastMeasurementTime(0),
    _firstMeasurementNotBeforeMs(0) {

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

#if defined(ESP32)
    analogSetPinAttenuation(_analogPin, ADC_11db);
#endif

    unsigned long staggerMs_ = (static_cast<unsigned long>(_powerPin) % 10UL) * SENSOR_STAGGER_MS;
    _firstMeasurementNotBeforeMs = millis() + STARTUP_STABILIZATION_MS + staggerMs_;

    eventLogger.logf("Sensor '%s' initialized (Analog: %d, Power: %d, first measure in ~%lus)",
        _NameValue, _analogPin, _powerPin,
        (STARTUP_STABILIZATION_MS + staggerMs_) / 1000UL);
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
        eventLogger.logf(EventLogger::Status::WARN, "Sensor '%s' is disabled, skipping measurement", _NameValue);
        return -1;
    }

    constexpr int DISCARD_READS = 3;
    constexpr int SAMPLE_COUNT = 9;
    constexpr int SAMPLE_DELAY_MS = 15;

    digitalWrite(_powerPin, HIGH);
    delay(300); // Settling

    // Erste instabile Reads verwerfen
    for (int i_ = 0; i_ < DISCARD_READS; ++i_) {
        (void)analogRead(_analogPin);
        delay(5);
    }

    int samples_[SAMPLE_COUNT];
    for (int i_ = 0; i_ < SAMPLE_COUNT; ++i_) {
        samples_[i_] = analogRead(_analogPin);
        delay(SAMPLE_DELAY_MS);
    }

    digitalWrite(_powerPin, LOW);

    // Einfache Sortierung für Median
    for (int i_ = 0; i_ < SAMPLE_COUNT - 1; ++i_) {
        for (int j_ = i_ + 1; j_ < SAMPLE_COUNT; ++j_) {
            if (samples_[j_] < samples_[i_]) {
                int t_ = samples_[i_];
                samples_[i_] = samples_[j_];
                samples_[j_] = t_;
            }
        }
    }

    int rawValue_ = samples_[SAMPLE_COUNT / 2]; // Median

    _lastRawValue = rawValue_;
    _lastMeasurementTime = millis();

    int percent_ = getMoisturePercent();
    eventLogger.logf(EventLogger::Status::DEBUG, "Sensor '%s': %d%% (%d raw)", _NameValue, percent_, rawValue_);

    if (percent_ >= 0 && percent_ < getThreshold()) {
        eventLogger.logf(EventLogger::Status::WARN,
            "Sensor '%s' below threshold (%d%% < %d%%)",
            _NameValue, percent_, getThreshold());
    }
    else if (percent_ >= 0) {
        eventLogger.logf(EventLogger::Status::OK,
            "Sensor '%s' above threshold (%d%% >= %d%%)",
            _NameValue, percent_, getThreshold());
    }

    return rawValue_;
}

int SoilMoistureSensor::getLastMoisture() const {
    return _lastRawValue;
}

int SoilMoistureSensor::getMoisturePercent() const {
    if (_lastRawValue < 0) return -1;

    // UI-Logik aktuell:
    // Min = dry, Max = wet
    int dry_ = getMinValue();
    int wet_ = getMaxValue();

    // Nur identische Werte sind ungültig (Division durch 0 in map)
    if (dry_ == wet_) return -1;

    int percent_ = map(_lastRawValue, dry_, wet_, 0, 100);
    return constrain(percent_, 0, 100);
}

unsigned long SoilMoistureSensor::getLastMeasurementTime() const {
    return _lastMeasurementTime;
}

bool SoilMoistureSensor::shouldMeasure() const {
    if (!isEnabled()) return false;

    if (_lastMeasurementTime == 0) {
        return millis() >= _firstMeasurementNotBeforeMs;
    }

    unsigned long elapsed_ = millis() - _lastMeasurementTime;
    return elapsed_ >= MEASUREMENT_INTERVAL_MS;
}

SoilMoistureSensor soilMoistureSensors[MAX_SOIL_SENSORS] = {
    SoilMoistureSensor("soilsensor1", "Soil Sensor 1", SOIL_MOISTURE_PIN_1, SOIL_MOISTURE_POWER_PIN_1),
    SoilMoistureSensor("soilsensor2", "Soil Sensor 2", SOIL_MOISTURE_PIN_2, SOIL_MOISTURE_POWER_PIN_2)
};