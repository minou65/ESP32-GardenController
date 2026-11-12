// RuntimeCounter.h
// Tracks accumulated relay ON time (operating hours) per zone.

#ifndef _RUNTIMECOUNTER_h
#define _RUNTIMECOUNTER_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

#include "Scheduler.h" // for MAX_SCHEDULERS

class RuntimeCounter {
public:
    void begin();

    // Call frequently (e.g. every loop or every second) with the current
    // physical relay state (true = ON) for every zone.
    void update(const bool relayState[MAX_SCHEDULERS]);

    // Total accumulated runtime in seconds for the given zone.
    unsigned long getRuntimeSeconds(uint8_t zone) const;

    // Human readable "Xd HH:MM:SS" formatted runtime.
    String getRuntimeString(uint8_t zone) const;

    // Reset runtime counter for a single zone / all zones.
    void resetZone(uint8_t zone);
    void resetAll();

private:
    void save(uint8_t zone);
    void load();

    unsigned long _accumulatedSeconds[MAX_SCHEDULERS] = { 0 };
    unsigned long _lastOnTimestampMs[MAX_SCHEDULERS] = { 0 };
    bool _wasOn[MAX_SCHEDULERS] = { false };
    unsigned long _lastPersistMs = 0;
};

extern RuntimeCounter runtimeCounter;

#endif