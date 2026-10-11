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
#include <EventLogger.h>
#include <RebootManager.h>

char Version[] = VERSION_STR; // Manufacturer's Software version code

static Neotimer schedulerTimer = Neotimer(5000); // 5 Sekunden
NTPClient ntpClient;

void setup() {
	Serial.begin(115200);
	while (!Serial) {
		delay(1);
	}

	eventLogger.logf("ESP32 Gardena Controller Version: %s", Version);

	// Load stored values, record the current reboot reason,
	// and increment the reboot counter.
	RebootManager::begin();

	eventLogger.logf(EventLogger::Status::DEBUG, "Reboot reason: %s", RebootManager::getLastRebootReasonText().c_str());
	eventLogger.logf(EventLogger::Status::DEBUG, "Reboot count: %d", RebootManager::getRebootCount());


	for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
		pinMode(RELAY_PINS[i_], OUTPUT);
		digitalWrite(RELAY_PINS[i_], RELAY_OFF);
		outputStatus[i_] = OUTPUT_AUTO;
		manualOnTimestamp[i_] = 0;
	}
	eventLogger.log(String(MAX_SCHEDULERS) + " relay outputs initialized", EventLogger::Status::OK);

	for (int i_ = 0; i_ < MAX_SOIL_SENSORS; i_++) {
		soilMoistureSensors[i_].begin();
	}
	eventLogger.log(String(MAX_SOIL_SENSORS) + " soil moisture sensors initialized", EventLogger::Status::OK);

	eventLogger.log("Starting web server setup...");
	setupWebServer();

	eventLogger.log("Setting up NTP client...");
	if (ntpConfig.useNtpServer()) {
		eventLogger.logf(EventLogger::Status::OK, "    NTP server: %s, Timezone: %s", ntpConfig.ntpServer().c_str(), ntpConfig.timeZone().c_str());
		ntpClient.begin(ntpConfig.ntpServer(), ntpConfig.timeZone(), 0);
	}
	else {
		eventLogger.log("    NTP disabled", EventLogger::Status::WARN);
	}
	
	eventLogger.log("Setup completed successfully", EventLogger::Status::OK);
}

void loop() {
	loopWebServer();

	if (iotWebConf.getState() == iotwebconf::OnLine) {
		if (ntpClient.isInitialized()) {
			ntpClient.process();
		}
	}

	// Soil moisture measurement at configured interval
	for (int i = 0; i < MAX_SOIL_SENSORS; i++) {
		if (soilMoistureSensors[i].shouldMeasure()) {
			eventLogger.logf(
				"AUTO measurement triggered for sensor %d (%s)",
				i + 1,
				soilMoistureSensors[i].getName()
			);

			int rawValue_ = soilMoistureSensors[i].measureMoisture();
			if (rawValue_ < 0) {
				eventLogger.logf(EventLogger::Status::ERROR,
					"AUTO measurement skipped/failed for sensor %d (%s)",
					i + 1,
					soilMoistureSensors[i].getName()
				);
			}
		}
	}

	// Check for manual timeout every loop
	checkManualTimeout();

	if (schedulerTimer.repeat() || updateOutputs) {

		updateOutputs = false;
		
		for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
			Scheduler& scheduler_ = schedulers[i_];
			
			// Aktuellen Pin-Status auslesen (VOR der Änderung)
			int previousState_ = digitalRead(RELAY_PINS[i_]);
			
			switch(outputStatus[i_]) {
				case OUTPUT_ON:
					digitalWrite(RELAY_PINS[i_], RELAY_ON);
					// Log nur bei Statusänderung
					if (previousState_ != RELAY_ON) {
						eventLogger.logf(EventLogger::Status::OK, "Zone %d (%s) turned ON (manual)", 
						                i_ + 1, scheduler_.getZone());
					}
					continue;
				case OUTPUT_OFF:
					digitalWrite(RELAY_PINS[i_], RELAY_OFF);
					// Log nur bei Statusänderung
					if (previousState_ != RELAY_OFF) {
						eventLogger.logf(EventLogger::Status::OK, "Zone %d (%s) turned OFF (manual)", 
						                i_ + 1, scheduler_.getZone());
					}
					continue;
				case OUTPUT_AUTO:
					// Automatikbetrieb, weiter unten prüfen
					break;
			}

			// Check if time is valid before automatic control
			// Without valid time, automatic irrigation is not allowed
			if (!ntpClient.isValidTime()) {
				digitalWrite(RELAY_PINS[i_], RELAY_OFF);
				// Log nur bei Statusänderung
				if (previousState_ != RELAY_OFF) {
					eventLogger.logf(EventLogger::Status::WARN, "Zone %d (%s) turned OFF (no valid time)", 
					                i_ + 1, scheduler_.getZone());
				}
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
							// Log nur bei Statusänderung
							if (previousState_ != RELAY_OFF) {
								eventLogger.logf(EventLogger::Status::OK, "Zone %d (%s) turned OFF (moisture %d%% >= %d%%)", 
								                i_ + 1, scheduler_.getZone(), 
								                moisturePercent_, threshold_);
							}
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
				// Log nur bei Statusänderung
				if (previousState_ != RELAY_ON) {
					eventLogger.logf(EventLogger::Status::OK, "Zone %d (%s) turned ON (scheduler)", 
					                i_ + 1, scheduler_.getZone());
				}
			}
			else {
				digitalWrite(RELAY_PINS[i_], RELAY_OFF);
				// Log nur bei Statusänderung
				if (previousState_ != RELAY_OFF) {
					eventLogger.logf(EventLogger::Status::OK, "Zone %d (%s) turned OFF (scheduler)", 
					                i_ + 1, scheduler_.getZone());
				}
			}
		}
	}

	if (ShouldReboot) {
		eventLogger.log("Rebooting...", EventLogger::Status::DEBUG);
		delay(500);
		ESP.restart();
	}
}
