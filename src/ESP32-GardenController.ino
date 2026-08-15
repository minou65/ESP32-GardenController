/*
 Name:		ESP32-GardenaController.ino
 Created:	16.07.2026 15:22:03
 Author:	andy
*/

#include "html.h"
#include "handlingWeb.h"
#include "Scheduler.h"
#include "Neotimer.h"
#include "version.h"


#include <Arduino.h>
#include <NTPClient.h>

char Version[] = VERSION_STR; // Manufacturer's Software version code

static Neotimer schedulerTimer = Neotimer(5000); // 5 Sekunden
NTPClient ntpClient;

void setup() {
	Serial.begin(115200);
	while (!Serial) {
		delay(1);
	}

	Serial.printf("ESP32 Gardena Controller Version: %s\n", Version);

	for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
		pinMode(RELAY_PINS[i_], OUTPUT);
		digitalWrite(RELAY_PINS[i_], RELAY_OFF);
		outputStatus[i_] = OUTPUT_AUTO;
		manualOnTimestamp[i_] = 0;
	}

	for (int i_ = 0; i_ < MAX_SOIL_SENSORS; i_++) {
		soilMoistureSensors[i_].begin();
	}

	Serial.println("Connecting to WiFi and setting up web server...");
	setupWebServer();

	Serial.println("Setting up NTP client...");
	if (ntpConfig.useNtpServer()) {
		Serial.printf("    Using NTP server: %s, Timezone: %s\n", ntpConfig.ntpServer().c_str(), ntpConfig.timeZone().c_str());
		ntpClient.begin(ntpConfig.ntpServer(), ntpConfig.timeZone(), 0);
	}
	else {
		Serial.println(F("    NTP not used"));
	}
}

void loop() {
	loopWebServer();

	if (iotWebConf.getState() == iotwebconf::OnLine) {
		if (ntpClient.isInitialized()) {
			ntpClient.process();
		}
	}

	// Soil moisture measurement every 12 hours
	for (int i = 0; i < MAX_SOIL_SENSORS; i++) {
		if (soilMoistureSensors[i].shouldMeasure()) {
			soilMoistureSensors[i].measureMoisture();
		}
	}

	// Check for manual timeout every loop
	checkManualTimeout();

	if (schedulerTimer.repeat() || updateOutputs) {

		updateOutputs = false;
		
		for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
			Scheduler& scheduler_ = schedulers[i_];
			switch(outputStatus[i_]) {
				case OUTPUT_ON:
					digitalWrite(RELAY_PINS[i_], RELAY_ON);
					//Serial.printf("Scheduler %s is manually ON\n", scheduler_.getZone());
					continue;
				case OUTPUT_OFF:
					digitalWrite(RELAY_PINS[i_], RELAY_OFF);
					//Serial.printf("Scheduler %s is manually OFF\n", scheduler_.getZone());
					continue;
				case OUTPUT_AUTO:
					// Automatikbetrieb, weiter unten prüfen
					break;
			}

			// Check if time is valid before automatic control
			// Without valid time, automatic irrigation is not allowed
			if (!ntpClient.isValidTime()) {
				digitalWrite(RELAY_PINS[i_], RELAY_OFF);
				continue;
			}
			
			// Check moisture sensor if configured for this zone
			int sensorIndex_ = scheduler_.getMoistureSensorIndex();
			if (sensorIndex_ >= 0 && sensorIndex_ < MAX_SOIL_SENSORS) {
				SoilMoistureSensor& sensor_ = soilMoistureSensors[sensorIndex_];

				if (sensor_.isEnabled()) {
					int moisturePercent_ = sensor_.getMoisturePercent();
					if (moisturePercent_ >= 0) {
						int threshold_ = sensor_.getThreshold();

						// If moisture is ABOVE threshold, soil is wet enough
						// -> DON'T water (continue)
						if (moisturePercent_ >= threshold_) {
							digitalWrite(RELAY_PINS[i_], RELAY_OFF);
							continue;
						}

						// If we reach here: moisture is BELOW threshold
						// -> Watering needed (proceed to schedule check)
					}
				}
			}
			
			// Logik: duration > 0, Tag korrekt, aktuelle Zeit zwischen Startzeit und (Startzeit + Duration)
			if (scheduler_.getDuration() > 0 &&
				scheduler_.isActiveOnDay(ntpClient.getDayOfWeek()) &&
				scheduler_.isActiveAtTime(ntpClient.getMinutesSinceMidnight())) {
				digitalWrite(RELAY_PINS[i_], RELAY_ON);
			}
			else {
				digitalWrite(RELAY_PINS[i_], RELAY_OFF);
			}
		}

	}

	if (ShouldReboot) {
		Serial.println("Rebooting...");
		delay(100);
		ESP.restart();
	}

}
