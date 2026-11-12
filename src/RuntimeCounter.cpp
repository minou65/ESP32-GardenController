// RuntimeCounter.cpp

#include "RuntimeCounter.h"
#include <Preferences.h>

#define RUNTIME_PREF_NAMESPACE "runtimeHrs"
#define RUNTIME_PERSIST_INTERVAL_MS (15UL * 60UL * 1000UL) // persist at most once every 15 minutes per zone

RuntimeCounter runtimeCounter;

void RuntimeCounter::begin() {
    load();
}

void RuntimeCounter::load() {
    Preferences pref_;
    pref_.begin(RUNTIME_PREF_NAMESPACE, true);

    for (uint8_t i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
        String key_ = "z" + String(i_ + 1);
        _accumulatedSeconds[i_] = pref_.getULong(key_.c_str(), 0);
    }

    pref_.end();
}

void RuntimeCounter::save(uint8_t zone) {
    if (zone >= MAX_SCHEDULERS) return;

    Preferences pref_;
    pref_.begin(RUNTIME_PREF_NAMESPACE, false);

    String key_ = "z" + String(zone + 1);
    pref_.putULong(key_.c_str(), _accumulatedSeconds[zone]);

    pref_.end();
}

void RuntimeCounter::update(const bool relayState[MAX_SCHEDULERS]) {
    unsigned long now_ = millis();

    for (uint8_t i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
        bool isOn_ = relayState[i_];

        if (isOn_ && !_wasOn[i_]) {
            // Rising edge: zone just turned ON
            _lastOnTimestampMs[i_] = now_;
        }
        else if (isOn_ && _wasOn[i_]) {
            // Still ON: accumulate elapsed time, then reset reference point
            unsigned long elapsedMs_ = now_ - _lastOnTimestampMs[i_];
            _accumulatedSeconds[i_] += elapsedMs_ / 1000UL;
            _lastOnTimestampMs[i_] = now_ - (elapsedMs_ % 1000UL);
        }
        else if (!isOn_ && _wasOn[i_]) {
            // Falling edge: zone just turned OFF, finalize and persist
            unsigned long elapsedMs_ = now_ - _lastOnTimestampMs[i_];
            _accumulatedSeconds[i_] += elapsedMs_ / 1000UL;
            save(i_);
        }

        _wasOn[i_] = isOn_;
    }

    // Periodically persist ongoing runtime for zones still ON (crash/power-loss safety)
    if (now_ - _lastPersistMs >= RUNTIME_PERSIST_INTERVAL_MS) {
        _lastPersistMs = now_;
        for (uint8_t i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
            if (_wasOn[i_]) {
                unsigned long elapsedMs_ = now_ - _lastOnTimestampMs[i_];
                _accumulatedSeconds[i_] += elapsedMs_ / 1000UL;
                _lastOnTimestampMs[i_] = now_ - (elapsedMs_ % 1000UL);
                save(i_);
            }
        }
    }
}

unsigned long RuntimeCounter::getRuntimeSeconds(uint8_t zone) const {
    if (zone >= MAX_SCHEDULERS) return 0;
    return _accumulatedSeconds[zone];
}

String RuntimeCounter::getRuntimeString(uint8_t zone) const {
    unsigned long totalSeconds_ = getRuntimeSeconds(zone);

    unsigned long days_ = totalSeconds_ / 86400UL;
    unsigned long hours_ = (totalSeconds_ % 86400UL) / 3600UL;
    unsigned long minutes_ = (totalSeconds_ % 3600UL) / 60UL;
    unsigned long seconds_ = totalSeconds_ % 60UL;

    char buffer_[32];
    snprintf(buffer_, sizeof(buffer_), "%lud %02lu:%02lu:%02lu", days_, hours_, minutes_, seconds_);
    return String(buffer_);
}

void RuntimeCounter::resetZone(uint8_t zone) {
    if (zone >= MAX_SCHEDULERS) return;

    _accumulatedSeconds[zone] = 0;
    _lastOnTimestampMs[zone] = millis();
    save(zone);
}

void RuntimeCounter::resetAll() {
    for (uint8_t i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
        resetZone(i_);
    }
}