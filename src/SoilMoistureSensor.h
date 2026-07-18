#ifndef _SOILMOISTURESENSOR_h
#define _SOILMOISTURESENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <IotWebConf.h>

#define STRING_LEN 32
#define NUMBER_LEN 5

#define SOIL_MOISTURE_PIN 36       // Analog Pin für Messung
#define SOIL_MOISTURE_POWER_PIN 33 // Digital Pin für Stromversorgung

class SoilMoistureSensor : public iotwebconf::ParameterGroup {
public:
    SoilMoistureSensor(const char* id, const char* label);
    void applyDefaultValue() override;

    const char* getName() const;
    int getMinValue() const;      // Trockener Boden
    int getMaxValue() const;      // Nasser Boden
    int getThreshold() const;     // Schwellwert zum Auslösen
    bool isEnabled() const;

    void begin();                 // Pin-Setup
    int measureMoisture();        // Messung durchführen (mit Power on/off)
    int getLastMoisture() const;  // Letzten gemessenen Wert abrufen
    int getMoisturePercent() const; // Als Prozent 0-100%

    unsigned long getLastMeasurementTime() const;
    bool shouldMeasure() const;   // Prüft ob 12h vergangen

private:
    String _id;
    int _lastRawValue;
    unsigned long _lastMeasurementTime;

    char _NameValue[STRING_LEN] = { 0 };
    char _MinValue[NUMBER_LEN] = { 0 };
    char _MaxValue[NUMBER_LEN] = { 0 };
    char _ThresholdValue[NUMBER_LEN] = { 0 };
    char _EnabledValue[STRING_LEN] = { 0 };

    char _NameId[STRING_LEN] = { 0 };
    char _MinId[STRING_LEN] = { 0 };
    char _MaxId[STRING_LEN] = { 0 };
    char _ThresholdId[STRING_LEN] = { 0 };
    char _EnabledId[STRING_LEN] = { 0 };

    iotwebconf::TextParameter _NameParam = iotwebconf::TextParameter("Name", _NameId, _NameValue, STRING_LEN);
    iotwebconf::NumberParameter _MinParam = iotwebconf::NumberParameter("Min Value (dry)", _MinId, _MinValue, NUMBER_LEN, "0", "0..4095", "min='0' max='4095' step='1'");
    iotwebconf::NumberParameter _MaxParam = iotwebconf::NumberParameter("Max Value (wet)", _MaxId, _MaxValue, NUMBER_LEN, "4095", "0..4095", "min='0' max='4095' step='1'");
    iotwebconf::NumberParameter _ThresholdParam = iotwebconf::NumberParameter("Threshold %", _ThresholdId, _ThresholdValue, NUMBER_LEN, "50", "0..100", "min='0' max='100' step='1'");
    iotwebconf::CheckboxParameter _EnabledParam = iotwebconf::CheckboxParameter("Enabled", _EnabledId, _EnabledValue, STRING_LEN, false);
};

extern SoilMoistureSensor soilMoistureSensor;

#endif