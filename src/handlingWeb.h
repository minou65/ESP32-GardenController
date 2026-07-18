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

#include <IotWebConfAsync.h>

extern bool ShouldReboot;
extern bool ConfigChanged;

// OutputStatus Enum
enum OutputStatus {
    OUTPUT_OFF = 0,
    OUTPUT_ON = 1,
    OUTPUT_AUTO = 2
};

extern OutputStatus outputStatus[MAX_SCHEDULERS];
extern AsyncIotWebConf iotWebConf;

void setupWebServer();
void loopWebServer();

#endif

