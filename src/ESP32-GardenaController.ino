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
	
	Serial.println("Setting up relays...");
	for (int i = 0; i < MAX_SCHEDULERS; i++) {
		pinMode(RELAY_PINS[i], OUTPUT);
		digitalWrite(RELAY_PINS[i], HIGH); // Alle Relais ausschalten (HIGH = OFF)
	}

	soilMoistureSensor.begin();

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
	if (soilMoistureSensor.shouldMeasure()) {
		soilMoistureSensor.measureMoisture();
	}

	if (schedulerTimer.repeat()) {
		
		for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
			Scheduler& scheduler_ = schedulers[i_];
			switch(outputStatus[i_]) {
				case OUTPUT_ON:
					digitalWrite(RELAY_PINS[i_], LOW);
					//Serial.printf("Scheduler %s is manually ON\n", scheduler_.getZone());
					continue;
				case OUTPUT_OFF:
					digitalWrite(RELAY_PINS[i_], HIGH);
					//Serial.printf("Scheduler %s is manually OFF\n", scheduler_.getZone());
					continue;
				case OUTPUT_AUTO:
					// Automatikbetrieb, weiter unten prüfen
					break;
			}

			if (soilMoistureSensor.isEnabled()) {
				int moisturePercent = soilMoistureSensor.getMoisturePercent();
				if (moisturePercent >= 0) {
					int threshold = soilMoistureSensor.getThreshold();

					// If moisture is ABOVE threshold, soil is wet enough
					// -> DON'T water (continue)
					if (moisturePercent >= threshold) {
						//Serial.printf("Soil moisture OK (%d%% >= %d%%), skipping scheduler %s\n",
						//	moisturePercent,
						//	threshold,
						//	scheduler_.getZone());
						continue;
					}

					// If we reach here: moisture is BELOW threshold
					// -> Watering needed
					//Serial.printf("Soil moisture low (%d%% < %d%%), scheduler %s will run\n",
					//	moisturePercent,
					//	threshold,
					//	scheduler_.getZone());
				}
			}
			
			//Serial.printf("Checking scheduler %s: StartTime=%s, Duration=%d, Weekdays=%u\n",
			//	scheduler_.getZone(),
			//	scheduler_.getStartTimeString(),
			//	scheduler_.getDuration(),
			//	scheduler_.getWeekdays());
			//Serial.printf("is active on day %d: %s\n", ntpClient.getDayOfWeek(), scheduler_.isActiveOnDay(ntpClient.getDayOfWeek()) ? "yes" : "no");
			
			// Logik: duration > 0, Tag korrekt, aktuelle Zeit zwischen Startzeit und (Startzeit + Duration)
			if (scheduler_.getDuration() > 0 &&
				scheduler_.isActiveOnDay(ntpClient.getDayOfWeek()) &&
				scheduler_.isActiveAtTime(ntpClient.getMinutesSinceMidnight())) {
				digitalWrite(RELAY_PINS[i_], LOW);
				//Serial.printf("Scheduler %s is active at %02d:%02d\n",
				//	scheduler_.getZone(),
				//	ntpClient.getHour(),
				//	ntpClient.getMinute());
			}
			else {
				digitalWrite(RELAY_PINS[i_], HIGH);
			}
		}

	}

	if (ShouldReboot) {
		Serial.println("Rebooting...");
		delay(100);
		ESP.restart();
	}

}
