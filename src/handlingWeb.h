// handlingWeb.h

#ifndef _HANDLINGWEB_h
#define _HANDLINGWEB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Scheduler.h"
#include "SoilMoistureSensor.h"
#include "NTPConfig.h"

#include <IotWebConfAsyncTab.h>

extern bool ShouldReboot;
extern bool ConfigChanged;
extern bool updateOutputs;

// OutputStatus Enum
enum OutputStatus {
    OUTPUT_OFF = 0,
    OUTPUT_ON = 1,
    OUTPUT_AUTO = 2
};

extern OutputStatus outputStatus[MAX_SCHEDULERS];
extern unsigned long manualOnTimestamp[MAX_SCHEDULERS];  // Timestamp when manually turned ON
extern AsyncIotWebConfTab iotWebConf;

void setupWebServer();
void loopWebServer();
void checkManualTimeout();  // Check for 30-minute timeout

#define MANUAL_TIMEOUT_MS (30UL * 60UL * 1000UL)  // 30 minutes in milliseconds

#endif

