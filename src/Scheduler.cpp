// Scheduler.cpp
// Implementation of the Scheduler class

#include "Scheduler.h"

// --- Scheduler ---
Scheduler::Scheduler(const char* id, const char* label)
    : _id(id),
    ParameterGroup(id, label) {
    snprintf(_ZoneValue, STRING_LEN, "");
    snprintf(_WeekdaysValue, STRING_LEN, "0,1,2,3,4,5,6");
    snprintf(_StartTimeValue, STRING_LEN, "00:00");
    snprintf(_DurationValue, NUMBER_LEN, "0");
    snprintf(_MoistureSensorValue, NUMBER_LEN, "0");

    snprintf(_ZoneId, STRING_LEN, "%s-zone", _id.c_str());
    snprintf(_WeekdaysId, STRING_LEN, "%s-weekdays", _id.c_str());
    snprintf(_StartTimeId, STRING_LEN, "%s-starttime", _id.c_str());
    snprintf(_DurationId, STRING_LEN, "%s-duration", _id.c_str());
    snprintf(_MoistureSensorId, STRING_LEN, "%s-moisturesensor", _id.c_str());

    this->addItem(&this->_ZoneParam);
    this->addItem(&this->_WeekdaysParam);
    this->addItem(&this->_StartTimeParam);
    this->addItem(&this->_DurationParam);
    this->addItem(&this->_MoistureSensorParam);
}

void Scheduler::applyDefaultValue() {
    _ZoneParam.applyDefaultValue();
    _WeekdaysParam.applyDefaultValue();
    _StartTimeParam.applyDefaultValue();
    _DurationParam.applyDefaultValue();
    _MoistureSensorParam.applyDefaultValue();
}

const char* Scheduler::getZone() const {
    return _ZoneValue;
}

uint8_t Scheduler::getWeekdays() const {
    uint8_t weekdaysMask = 0;

    // Parse "0,1,2,3,4,5,6" und setze entsprechende Bits
    String weekdaysStr = String(_WeekdaysValue);
    int startPos = 0;
    int commaPos = 0;

    while (commaPos != -1) {
        commaPos = weekdaysStr.indexOf(',', startPos);
        String dayStr = (commaPos != -1)
            ? weekdaysStr.substring(startPos, commaPos)
            : weekdaysStr.substring(startPos);

        int day = dayStr.toInt();
        if (day >= 0 && day <= 6) {
            weekdaysMask |= (1 << day);  // Setze Bit für diesen Tag
        }

        startPos = commaPos + 1;
    }

    return weekdaysMask;
}

const char* Scheduler::getStartTimeString() const {
    return _StartTimeValue;
}

int Scheduler::getStartTimeMinutes() const {
    int h_ = 0, m_ = 0;
    sscanf(_StartTimeValue, "%2d:%2d", &h_, &m_);
    return h_ * 60 + m_;
}

int Scheduler::getDuration() const {
    return atoi(_DurationValue);
}

int Scheduler::getMoistureSensorIndex() const {
    int value = atoi(_MoistureSensorValue);
    return value - 1;  // 0 -> -1 (none), 1 -> 0 (sensor 1), 2 -> 1 (sensor 2)
}

bool Scheduler::isActiveOnDay(uint8_t dayOfWeek) const {
    // dayOfWeek: 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    // or 1 = Monday, ..., 7 = Sunday (depending on your system)
    // Assuming: 0 = Monday, 1 = Tuesday, ..., 6 = Sunday (ISO 8601)
    if (dayOfWeek > 6) return false;

    uint8_t weekdays_ = getWeekdays();
    return (weekdays_ & (1 << dayOfWeek)) != 0;
}

bool Scheduler::isActiveAtTime(int currentMinutes) const {
    int duration_ = getDuration();

    // Nur aktiv wenn duration > 0
    if (duration_ <= 0) {
        return false;
    }

    int startMinutes_ = getStartTimeMinutes();
    int endMinutes_ = startMinutes_ + duration_;

    // Handle wrap-around past midnight
    if (endMinutes_ >= 1440) { // 24 * 60 = 1440 minutes in a day
        // Active if current time is after start OR before wrapped end
        return (currentMinutes >= startMinutes_) || (currentMinutes < (endMinutes_ % 1440));
    }

    // Normal case: no wrap-around
    return (currentMinutes >= startMinutes_) && (currentMinutes < endMinutes_);
}

Scheduler schedulers[MAX_SCHEDULERS] = { 
	Scheduler("scheduler1", "Zone 1"),
	Scheduler("scheduler2", "Zone 2"),
	Scheduler("scheduler3", "Zone 3"),
	Scheduler("scheduler4", "Zone 4"),
	Scheduler("scheduler5", "Zone 5"),
	Scheduler("scheduler6", "Zone 6")
};