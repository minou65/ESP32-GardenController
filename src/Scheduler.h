// Scheduler.h

#ifndef _SCHEDULER_h
#define _SCHEDULER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <IotWebConf.h>

#define STRING_LEN 32
#define NUMBER_LEN 5

#define MAX_SCHEDULERS 6

// ESP32 8-Channel Relay Board 303E32DC810 Pin Configuration
const int RELAY_PINS[MAX_SCHEDULERS] = {
    32,  // Relay 1
    33,  // Relay 2
    25,  // Relay 3
    26,  // Relay 4
    27,  // Relay 5
    14  // Relay 6
};

// Moisture sensor dropdown options - defined here so sizeof works
static const char MoistureSensorValues[][NUMBER_LEN] = {
    "0",
    "1",
    "2"
};

static const char MoistureSensorNames[][STRING_LEN] = {
    "None",
    "Sensor 1",
    "Sensor 2"
};

#define RELAY_ON HIGH  // Relay is ON when pin is HIGH
#define RELAY_OFF LOW  // Relay is OFF when pin is LOW

class Scheduler : public iotwebconf::ParameterGroup {
public:
    Scheduler(const char* id, const char* label);
    void applyDefaultValue() override;

    const char* getZone() const;
    uint8_t getWeekdays() const;
    const char* getStartTimeString() const;
    int getStartTimeMinutes() const;
    int getDuration() const;
    int getMoistureSensorIndex() const;

    bool isActiveOnDay(uint8_t dayOfWeek) const;
    bool isActiveAtTime(int currentMinutes) const;

private:
    String _id;

    char _ZoneValue[STRING_LEN] = { 0 };
    char _WeekdaysValue[STRING_LEN] = { 0 };
    char _StartTimeValue[STRING_LEN] = { 0 };
    char _DurationValue[NUMBER_LEN] = { 0 };
    char _MoistureSensorValue[NUMBER_LEN] = { 0 };

    char _ZoneId[STRING_LEN] = { 0 };
    char _WeekdaysId[STRING_LEN] = { 0 };
    char _StartTimeId[STRING_LEN] = { 0 };
    char _DurationId[STRING_LEN] = { 0 };
    char _MoistureSensorId[STRING_LEN] = { 0 };

    iotwebconf::TextParameter _ZoneParam = iotwebconf::TextParameter("Name", _ZoneId, _ZoneValue, STRING_LEN);
    iotwebconf::DayParameter _WeekdaysParam = iotwebconf::DayParameter("Weekdays", _WeekdaysId, _WeekdaysValue, STRING_LEN, true, "0,1,2,3,4,5,6");
    iotwebconf::TimeParameter _StartTimeParam = iotwebconf::TimeParameter("Start Time", _StartTimeId, _StartTimeValue, STRING_LEN, "00:00");
    iotwebconf::NumberParameter _DurationParam = iotwebconf::NumberParameter("Duration (Minutes)", _DurationId, _DurationValue, NUMBER_LEN, "0", "0..240", "min='0' max='240' step='1'");
    iotwebconf::SelectParameter _MoistureSensorParam = iotwebconf::SelectParameter(
        "Moisture Sensor", _MoistureSensorId, _MoistureSensorValue, sizeof(MoistureSensorValues[0]),
        (char*)MoistureSensorValues, (char*)MoistureSensorNames,
        sizeof(MoistureSensorValues) / sizeof(MoistureSensorValues[0]), sizeof(MoistureSensorNames[0]),
        MoistureSensorNames[0]
    );
};

extern Scheduler schedulers[MAX_SCHEDULERS];

#endif

