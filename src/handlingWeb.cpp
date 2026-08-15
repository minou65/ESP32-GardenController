// 
// 
// 

#include "handlingWeb.h"
#include "html.h"
#include "favicon.h"

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <DNSServer.h>

#include <IotWebConfAsyncUpdateServer.h>
#include <IotWebConfAsyncTab.h>
#include <IotWebRootTab.h>
#include <Preferences.h>


// -- Initial password to connect to the Thing, when it creates an own Access Point.
const char wifiInitialApPassword[] = "123456789";

// -- Configuration specific key. The value should be modified if config structure was changed.
#define CONFIG_VERSION "001"
extern char Version[];

// -- When CONFIG_PIN is pulled to ground on startup, the Thing will use the initial
//      password to buld an AP. (E.g. in case of lost password)
#define CONFIG_PIN  -1

// -- Status indicator pin.
//      First it will light up (kept LOW), on Wifi connection it will blink,
//      when connected to the Wifi it will turn off (kept HIGH).
#define STATUS_PIN GPIO_NUM_23
#define ON_LEVEL HIGH

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "GardenController";

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebServerWrapper asyncWebServerWrapper(&server);
AsyncUpdateServer AsyncUpdater;

NtpConfig ntpConfig = NtpConfig();

AsyncIotWebConfTab iotWebConf(thingName, &dnsServer, &asyncWebServerWrapper, wifiInitialApPassword, CONFIG_VERSION);

OutputStatus outputStatus[MAX_SCHEDULERS]{};
unsigned long manualOnTimestamp[MAX_SCHEDULERS] = {};

bool ShouldReboot = false;
bool ConfigChanged = false;
bool updateOutputs = false;

extern const int RELAY_PINS[];

// -- Method declarations.
void handleRoot(AsyncWebServerRequest* request);
void handleDateTime(AsyncWebServerRequest* request);
void handleData(AsyncWebServerRequest* request);
void handlePost(AsyncWebServerRequest* request);
void handleAPPasswordMissingPage(iotwebconf::WebRequestWrapper* webRequestWrapper);
void handleSSIDNotConfiguredPage(iotwebconf::WebRequestWrapper* webRequestWrapper);
void handleConfigSaved();
void handleConfigSavedPage(iotwebconf::WebRequestWrapper* webRequestWrapper);
void handleConnectWifi(const char* ssid, const char* password);
iotwebconf::WifiAuthInfo* handleConnectWifiFailure();
void handleWificonnected();
void handleWebSerialCommand(const String& command);
void handleOnProgress(size_t prg, size_t sz);
void handleReboot(AsyncWebServerRequest* request);
void handleMeasure(AsyncWebServerRequest* request);

class CustomHtmlFormatProvider : public AsyncTabHtmlFormatProvider {
public:
    CustomHtmlFormatProvider(std::vector<AsyncTabInfo>* tabs, int minWidth = 500, int maxWidth = 600)
        : AsyncTabHtmlFormatProvider(tabs, minWidth, maxWidth) {
    }

protected:
    String getFormEnd() override {
        return AsyncTabHtmlFormatProvider::getFormEnd() + String(FPSTR(html_form_end));
    }
    String getEnd() override {
        return String(FPSTR(html_end_template)) + AsyncTabHtmlFormatProvider::getEnd();
    }
};
CustomHtmlFormatProvider* customHtmlFormatProvider = nullptr;

class MyHtmlRootFormatProvider : public TabHtmlRootFormatProvider {
public:
    String getHtmlTableRowClass(String name, String htmlclass, String id) {
        String s_ = F("<tr><td align=\"left\">{n}</td><td align=\"left\"><span id=\"{id}\" class=\"{c}\"></span></td></tr>\n");
        s_.replace("{n}", name);
        s_.replace("{c}", htmlclass);
        s_.replace("{id}", id);
        return s_;
    }

    String getOutputControlRow(const String& label, const String& id, OutputStatus status, int value, bool hasInputField) {
        String html_ = "<tr><td>" + label + "</td><td>";
        html_ += "<div class='btn-group' id='Channel" + id + "Btns'>";
        html_ += "<button class='output-btn on" + String(status == OUTPUT_ON ? " active" : "") + "' data-ch='" + id + "' data-state='on'>On</button>";
        html_ += "<button class='output-btn off" + String(status == OUTPUT_OFF ? " active" : "") + "' data-ch='" + id + "' data-state='off'>Off</button>";
        html_ += "<button class='output-btn auto" + String(status == OUTPUT_AUTO ? " active" : "") + "' data-ch='" + id + "' data-state='auto'>Auto</button>";
        html_ += "</div></td></tr>";
        return html_;
    }

protected:
    String getStyleInner() override {
        String s_ = TabHtmlRootFormatProvider::getStyleInner();
        s_ += String(FPSTR(html_output_styles));
        return s_;
    }

    String getScriptInner() override {
        String s_ = TabHtmlRootFormatProvider::getScriptInner();

        s_.replace("{millisecond}", "10000");
        s_ += String(FPSTR(html_script));
        return s_;
    }
};

void saveOutputStatus() {
    Preferences pref_;
    pref_.begin("outputStatus", false);

    for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
        String key_ = "ch" + String(i_ + 1);
        pref_.putUChar(key_.c_str(), static_cast<uint8_t>(outputStatus[i_]));
    }

    pref_.end();
	Serial.println(F("Output status saved to preferences."));
}

void loadOutputStatus() {
    Preferences pref_;
	bool shouldsave_ = false;
    pref_.begin("outputStatus", true);

    for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
        String key_ = "ch" + String(i_ + 1);
        if (pref_.isKey(key_.c_str())) {
            outputStatus[i_] = static_cast<OutputStatus>(pref_.getUChar(key_.c_str(), static_cast<uint8_t>(OUTPUT_AUTO)));
            if (outputStatus[i_] == OUTPUT_ON) {
				outputStatus[i_] = OUTPUT_AUTO; // Reset to AUTO on load to avoid unintended manual ON state
				shouldsave_ = true;
			}
        }
    }

    pref_.end();

    if (shouldsave_) {
        saveOutputStatus(); // Save the reset state back to preferences
	}

	Serial.println(F("Output status loaded from preferences."));
}


void setupWebServer() {

	loadOutputStatus();

    iotWebConf.setStatusPin(STATUS_PIN, ON_LEVEL);
    iotWebConf.setConfigPin(CONFIG_PIN);
    
    iotWebConf.setSystemTabName("System");
    iotWebConf.setSystemTabPosition(-1);

    iotWebConf.addParameterGroup(&ntpConfig, "System");

    // Schedulers zu einem eigenen Tab
    for (int i_ = 0; i_ < MAX_SCHEDULERS; ++i_) {
        iotWebConf.addParameterGroup(&schedulers[i_], "Schedulers");
    }

    // Soil Moisture Sensors zu einem eigenen Tab
    for (int i_ = 0; i_ < MAX_SOIL_SENSORS; i_++) {
        iotWebConf.addParameterGroup(&soilMoistureSensors[i_], "Sensors");
    }

    auto customProvider = new CustomHtmlFormatProvider(iotWebConf.getTabsVector());
	customProvider->setContainerWidth(700, 700); // Set a fixed width for the configuration portal
    iotWebConf.setHtmlFormatProvider(customProvider);

    iotWebConf.getApTimeoutParameter()->visible = true;

    iotWebConf.setConfigSavedCallback(&handleConfigSaved);
    iotWebConf.setConfigSavedPage(&handleConfigSavedPage);

    iotWebConf.setConfigAPPasswordMissingPage(&handleAPPasswordMissingPage);
    iotWebConf.setConfigSSIDNotConfiguredPage(&handleSSIDNotConfiguredPage);

    iotWebConf.setWifiConnectionCallback(&handleWificonnected);
    iotWebConf.setWifiConnectionHandler(&handleConnectWifi);
    iotWebConf.setWifiConnectionFailedHandler(&handleConnectWifiFailure);

    iotWebConf.init();

    iotWebConf.setupUpdateServer(
        [](const char* updatePath) { AsyncUpdater.setup(&server, updatePath, handleOnProgress); },
        [](const char* userName, char* password) { AsyncUpdater.updateCredentials(userName, password); 
        }
    );

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleRoot(request);
        }
    );

    server.on("/config", HTTP_ANY, [](AsyncWebServerRequest* request) {
        auto* asyncWebRequestWrapper_ = new AsyncWebRequestWrapper(request);
        iotWebConf.handleConfig(asyncWebRequestWrapper_);
        }
    );

    IconHandler::registerHandlers(&server, favicon_ico, sizeof(favicon_ico));

    server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest* request) {
		handleReboot(request);
        }
    );

    server.on("/DateTime", HTTP_GET, [](AsyncWebServerRequest* request) { 
        handleDateTime(request); 
        }
    );
    
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest* request) { 
        handleData(request); 
        }
    );

    server.on("/post", HTTP_ANY, [](AsyncWebServerRequest* request) {
        handlePost(request);
        }
    );

    server.on("/measure", HTTP_POST, [](AsyncWebServerRequest* request) {
        handleMeasure(request);
    });

    server.onNotFound([](AsyncWebServerRequest* request) {
        AsyncWebRequestWrapper asyncWebRequestWrapper_(request);
        iotWebConf.handleNotFound(&asyncWebRequestWrapper_);
        }
    );

}

void loopWebServer() {
    iotWebConf.doLoop();

    if (!ShouldReboot) {
        ShouldReboot = AsyncUpdater.isFinished();
    }
}


void handleRoot(AsyncWebServerRequest* request) {
    AsyncWebRequestWrapper asyncWebRequestWrapper(request);
    if (iotWebConf.handleCaptivePortal(&asyncWebRequestWrapper)) {
        return;
    }

    AsyncResponseStream* response_ = request->beginResponseStream("text/html; charset=UTF-8");
    MyHtmlRootFormatProvider fp_;

    // === Create 3 Tabs ===
    fp_.addTab("Status", "status");
    fp_.addTab("Control", "control");
    fp_.addTab("System", "system");

    // Header
    response_->print(fp_.getHtmlHead(iotWebConf.getThingName()));
    response_->print(fp_.getIconLinks());
    response_->print(fp_.getHtmlStyle());
    response_->print(fp_.getHtmlHeadEnd());
    response_->print(fp_.getHtmlScript());
    response_->print(fp_.getHtmlTabButtons());

    // === TAB 1: Status (Zones & Sensors) ===
    response_->print(fp_.getHtmlTabContentStart("status"));

    // Time and RSSI in one row
    response_->print(F("<fieldset align=left style=\"border: 1px solid\">"));
    response_->print(F("<table border=\"0\" align=\"center\" width=\"100%\">"));
    response_->print(F("<tr><td align=\"left\"><span id=\"DateTimeValue\">Loading...</span></td><td align=\"right\"><span id=\"RSSIValue\">no data</span></td></tr>"));
    response_->print(F("</table></fieldset>"));

    // Soil Moisture Sensors (if any enabled)
    bool hasSensors_ = false;
    for (int i = 0; i < MAX_SOIL_SENSORS; i++) {
        if (soilMoistureSensors[i].isEnabled()) {
            hasSensors_ = true;
            break;
        }
    }

    if (hasSensors_) {
        response_->print(fp_.getHtmlFieldset("Soil Moisture Sensors"));
        response_->print(fp_.getHtmlTable());
        for (int i = 0; i < MAX_SOIL_SENSORS; i++) {
            if (soilMoistureSensors[i].isEnabled()) {
                String sensorName = String(soilMoistureSensors[i].getName());
                if (sensorName.length() == 0) {
                    sensorName = "Soil Sensor " + String(i + 1);
                }
                response_->print(F("<tr><td align=\"left\">"));
                response_->print(sensorName);
                response_->print(F(":</td><td align=\"right\"><span id=\"SoilMoisture"));
                response_->print(String(i + 1));
                response_->print(F("Percent\">N/A</span></td></tr>"));
            }
        }
        response_->print(fp_.getHtmlTableEnd());
        response_->print(fp_.getHtmlFieldsetEnd());
    }

    // Zone Status
    response_->print(fp_.getHtmlFieldset("Zone Status"));
    response_->print(fp_.getHtmlTable());
    for (uint8_t i = 0; i < MAX_SCHEDULERS; i++) {
        String zoneName = String(schedulers[i].getZone());
        if (zoneName.length() == 0) {
            zoneName = "Zone " + String(i + 1);
        }
        response_->print(F("<tr><td align=\"left\">"));
        response_->print(zoneName);
        response_->print(F(":</td><td align=\"right\"><span id=\"Zone"));
        response_->print(String(i + 1));
        response_->print(F("Status\">OFF</span></td></tr>"));
    }
    response_->print(fp_.getHtmlTableEnd());
    response_->print(fp_.getHtmlFieldsetEnd());

    response_->print(fp_.getHtmlTabContentEnd());

    // === TAB 2: Manual Control ===
    response_->print(fp_.getHtmlTabContentStart("control"));

    // Zone Control
    response_->print(fp_.getHtmlFieldset("Manual Zone Control"));
    response_->print(fp_.getHtmlTable());

    for (uint8_t ch_ = 0; ch_ < MAX_SCHEDULERS; ++ch_) {
        String zoneName = String(schedulers[ch_].getZone());
        if (zoneName.length() == 0) {
            zoneName = "Zone " + String(ch_ + 1);
        }
        response_->print(fp_.getOutputControlRow(zoneName, String(ch_ + 1), outputStatus[ch_], 0, false));
    }

    response_->print(fp_.getHtmlTableEnd());
    response_->print(fp_.getHtmlFieldsetEnd());

    // Check if any sensors are enabled
    for (int i = 0; i < MAX_SOIL_SENSORS; i++) {
        if (soilMoistureSensors[i].isEnabled()) {
            response_->print(fp_.getHtmlFieldset("Soil Moisture Sensors"));
            response_->print(fp_.getHtmlTable());
            
            // Loop again to add all enabled sensors
            for (int j = 0; j < MAX_SOIL_SENSORS; j++) {
                if (soilMoistureSensors[j].isEnabled()) {
                    String sensorName = String(soilMoistureSensors[j].getName());
                    if (sensorName.length() == 0) {
                        sensorName = "Soil Sensor " + String(j + 1);
                    }
                    response_->print(F("<tr><td align=\"left\">"));
                    response_->print(sensorName);
                    response_->print(F(":</td><td align=\"right\">"));
                    response_->print(F("<button class='measure-now-btn' data-sensor='"));
                    response_->print(String(j + 1));
                    response_->print(F("'>Measure Now</button>"));
                    response_->print(F("</td></tr>"));
                }
            }
            
            response_->print(fp_.getHtmlTableEnd());
            response_->print(fp_.getHtmlFieldsetEnd());
            break; // Exit outer loop after adding fieldset
        }
    }

    response_->print(fp_.getHtmlTabContentEnd());

    // === TAB 3: System Info ===
    response_->print(fp_.getHtmlTabContentStart("system"));

    response_->print(fp_.getHtmlFieldset("Network"));
    response_->print(fp_.getHtmlTable());

    uint64_t chipid_ = ESP.getEfuseMac();
    char macStr_[18];
    snprintf(macStr_, sizeof(macStr_), "%02X:%02X:%02X:%02X:%02X:%02X",
        (uint8_t)(chipid_ >> 40), (uint8_t)(chipid_ >> 32),
        (uint8_t)(chipid_ >> 24), (uint8_t)(chipid_ >> 16),
        (uint8_t)(chipid_ >> 8), (uint8_t)(chipid_));
    response_->print(fp_.getHtmlTableRowText("MAC Address:", String(macStr_)));

    String ipAddress_;
    if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
        ipAddress_ = WiFi.softAPIP().toString();
    }
    else {
        ipAddress_ = WiFi.localIP().toString();
    }
    if (ipAddress_ == "0.0.0.0" || ipAddress_.isEmpty()) {
        ipAddress_ = "Not connected";
    }
    response_->print(fp_.getHtmlTableRowText("IP Address:", ipAddress_));

    response_->print(fp_.getHtmlTableEnd());
    response_->print(fp_.getHtmlFieldsetEnd());

    response_->print(fp_.getHtmlTabContentEnd());

    // Footer
    response_->print(fp_.addNewLine(2));
    response_->print(fp_.getHtmlTable());
    response_->print(fp_.getHtmlTableRowText("Open <a href='/config'>configuration page</a>."));
    response_->print(fp_.getHtmlTableRowText("Open <a href='/webserial' target='_blank'>WebSerial Console</a> for live logs."));
    response_->print(fp_.getHtmlTableRowText(fp_.getHtmlVersion(Version)));
    response_->print(fp_.getHtmlTableEnd());

    response_->print(fp_.getBodyEnd());
    response_->print(fp_.getHtmlEnd());

    response_->addHeader("Server", "ESP Async Web Server");
    request->send(response_);
}

void handleDateTime(AsyncWebServerRequest* request) {
    ESP_LOGD("handleDateTime", "Time requested");

    // Get the current time as Unix timestamp (milliseconds)
    time_t now_ = time(nullptr);
    struct tm* timeinfo_ = localtime(&now_);

    // Send as formatted ISO string: "YYYY-MM-DD HH:MM:SS"
    char dateTimeStr_[32];
    strftime(dateTimeStr_, sizeof(dateTimeStr_), "%Y-%m-%d %H:%M:%S", timeinfo_);

    request->send(200, "text/plain", dateTimeStr_);
}

void handleData(AsyncWebServerRequest* request) {
    // JSON mit aktuellen Werten für AJAX-Updates
    String json_ = "{";
    json_ += "\"RSSI\":\"" + String(WiFi.RSSI()) + "\"";

    // Valve states (actual physical relay state)
    json_ += ",\"valve\":{";
    for (uint8_t i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
        if (i_ > 0) json_ += ",";
        int pinState_ = digitalRead(RELAY_PINS[i_]);
        String status_ = (pinState_ == RELAY_ON) ? "ON" : "OFF";
        json_ += "\"zone" + String(i_ + 1) + "\":\"" + status_ + "\"";
    }
    json_ += "}";

    // Button states (on/off/auto mode)
    json_ += ",\"button\":{";
    for (uint8_t i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
        if (i_ > 0) json_ += ",";
        String buttonState_;
        switch (outputStatus[i_]) {
        case OUTPUT_ON:   buttonState_ = "on"; break;
        case OUTPUT_OFF:  buttonState_ = "off"; break;
        case OUTPUT_AUTO: buttonState_ = "auto"; break;
        default:          buttonState_ = "auto"; break;
        }
        json_ += "\"zone" + String(i_ + 1) + "\":\"" + buttonState_ + "\"";
    }
    json_ += "}";

    // Add soil moisture sensors data
    json_ += ",\"moisture\":{";
    for (int i = 0; i < MAX_SOIL_SENSORS; i++) {
        if (i > 0) json_ += ",";
        if (soilMoistureSensors[i].isEnabled()) {
            int soilMoisturePercent_ = soilMoistureSensors[i].getMoisturePercent();
            json_ += "\"sensor" + String(i + 1) + "\":\"" + String(soilMoisturePercent_) + "\"";
        }
        else {
            json_ += "\"sensor" + String(i + 1) + "\":null";
        }
    }
    json_ += "}";

    json_ += "}";

    request->send(200, "application/json", json_);
}

void handlePost(AsyncWebServerRequest* request) {
    int params_ = request->params();

    if (request->hasParam("reset", true)) {

		ntpConfig.applyDefaultValue();

        for (int i_ = 0; i_ < MAX_SOIL_SENSORS; i_++) {
            soilMoistureSensors[i_].applyDefaultValue();
		}

        for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
            schedulers[i_].applyDefaultValue();
		}

        Serial.println("Resetting output status to AUTO");
        for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
            outputStatus[i_] = OUTPUT_AUTO;
            digitalWrite(RELAY_PINS[i_], RELAY_OFF); // Set all relays to OFF
        }
        saveOutputStatus();
        request->send(200, "text/plain", "Output status reset to AUTO");
        return;
	}

    for (int i_ = 0; i_ < params_; i_++) {
        const AsyncWebParameter* p = request->getParam(i_);
        if (p->isPost()) {
            String name_ = p->name();
            String value_ = p->value();

            Serial.printf("POST parameter: %s = %s\n", name_.c_str(), value_.c_str());

            for (int z_ = 1; z_ <= 8; z_++) {
                if (name_ == "ch" + String(z_)) {
                    int channel_ = z_ - 1;
                    if (value_ == "on") {
                        digitalWrite(RELAY_PINS[channel_], RELAY_ON);
                        Serial.printf("Zone %d turned ON manually - 30min timeout started\n", z_);
                        Serial.printf("Relay pin %d set to RELAY_ON\n", RELAY_PINS[channel_]);
                        outputStatus[channel_] = OUTPUT_ON;
                        manualOnTimestamp[channel_] = millis();  // Start timeout timer
                    }
                    else if (value_ == "off") {
                        digitalWrite(RELAY_PINS[channel_], RELAY_OFF);
                        Serial.printf("Zone %d turned OFF\n", z_);
                        Serial.printf("Relay pin %d set to RELAY_OFF\n", RELAY_PINS[channel_]);
                        outputStatus[channel_] = OUTPUT_OFF;
                        manualOnTimestamp[channel_] = 0;  // Clear timeout timer
                    }
                    else if (value_ == "auto") {
                        Serial.printf("Zone %d set to AUTO\n", z_);
                        outputStatus[channel_] = OUTPUT_AUTO;
                        manualOnTimestamp[channel_] = 0;  // Clear timeout timer
                    }
                }
            }
			saveOutputStatus();
			updateOutputs = true;  // Flag to indicate that outputs have changed and need to be updated
        }
    }

    request->send(200, "text/plain", "OK");
}

void handleAPPasswordMissingPage(iotwebconf::WebRequestWrapper* webRequestWrapper) {
    String html = "<!DOCTYPE html><html><head><title>Fehler</title></head><body>";
    html += "<h1>AP-Passwort fehlt</h1>";
    html += "<p>Bitte konfigurieren Sie ein Access Point Passwort.</p>";
    html += "<p><a href='/config'>Zur Konfiguration</a></p>";
    html += "</body></html>";

    webRequestWrapper->sendContent(html);
}

void handleSSIDNotConfiguredPage(iotwebconf::WebRequestWrapper* webRequestWrapper) {
    String html = "<!DOCTYPE html><html><head><title>Fehler</title></head><body>";
    html += "<h1>SSID nicht konfiguriert</h1>";
    html += "<p>Bitte konfigurieren Sie die WLAN-Zugangsdaten.</p>";
    html += "<p><a href='/config'>Zur Konfiguration</a></p>";
    html += "</body></html>";

    webRequestWrapper->sendContent(html);
}

void handleConfigSaved() {
    Serial.println("Config saved");
    ConfigChanged = true;
}

void handleConfigSavedPage(iotwebconf::WebRequestWrapper* webRequestWrapper) {
    webRequestWrapper->sendHeader("Location", "/", true);
    webRequestWrapper->send(302, "text/plain", "Config saved");
}

void handleConnectWifi(const char* ssid, const char* password) {
    Serial.println("Connecting to WiFi ...");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
}

iotwebconf::WifiAuthInfo* handleConnectWifiFailure() {
    Serial.println("WLAN-Verbindung fehlgeschlagen");

    static iotwebconf::WifiAuthInfo auth_;
    auth_ = iotWebConf.getWifiAuthInfo();
    return &auth_;
}

void handleWificonnected() {
    Serial.println("WLAN verbunden");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());
    ArduinoOTA.begin();
}

void handleWebSerialCommand(const String& command) {
    Serial.print("WebSerial Kommando: ");
    Serial.println(command);

    // Kommandos verarbeiten
    if (command == "reboot") {
        ShouldReboot = true;
    }
}

void handleOnProgress(size_t prg, size_t sz) {
    static size_t lastPrinted_ = 0;
    size_t currentPercent_ = (prg * 100) / sz;

    if (currentPercent_ % 5 == 0 && currentPercent_ != lastPrinted_) {
        Serial.printf("Progress: %d%%\n", currentPercent_);
        lastPrinted_ = currentPercent_;
    }
}

void handleReboot(AsyncWebServerRequest* request) {
    AsyncWebServerResponse* response_ = request->beginResponse(200, "text/html",
        "<html>"
        "<head>"
        "<meta http-equiv=\"refresh\" content=\"15; url=/\">"
        "<title>Rebooting...</title>"
        "</head>"
        "<body>"
        "Please wait while the device is rebooting...<br>"
        "You will be redirected to the homepage shortly."
        "</body>"
        "</html>");
    request->client()->setNoDelay(true); // Disable Nagle's algorithm so the client gets the response immediately
    request->send(response_);
    ShouldReboot = true;
}

void handleMeasure(AsyncWebServerRequest* request) {
    String json = "{";
    
    if (request->hasParam("sensor", true)) {
        String sensorParam = request->getParam("sensor", true)->value();
        int sensorId = sensorParam.toInt() - 1; // Convert to 0-based index
        
        if (sensorId >= 0 && sensorId < MAX_SOIL_SENSORS) {
            if (soilMoistureSensors[sensorId].isEnabled()) {
                int rawValue = soilMoistureSensors[sensorId].measureMoisture();
                int percent = soilMoistureSensors[sensorId].getMoisturePercent();
                
                json += "\"success\":true,";
                json += "\"sensor\":" + String(sensorId + 1) + ",";
                json += "\"raw\":" + String(rawValue) + ",";
                json += "\"percent\":" + String(percent);
                
                Serial.printf("Manual measurement: Sensor %d = %d%% (%d raw)\n", 
                    sensorId + 1, percent, rawValue);
            } else {
                json += "\"success\":false,";
                json += "\"error\":\"Sensor is disabled\"";
            }
        } else {
            json += "\"success\":false,";
            json += "\"error\":\"Invalid sensor ID\"";
        }
    } else {
        json += "\"success\":false,";
        json += "\"error\":\"Missing sensor parameter\"";
    }
    
    json += "}";
    request->send(200, "application/json", json);
}

void checkManualTimeout() {
    unsigned long currentMillis = millis();

    for (int i = 0; i < MAX_SCHEDULERS; i++) {
        // Check if zone is manually ON and timeout has expired
        if (outputStatus[i] == OUTPUT_ON && manualOnTimestamp[i] > 0) {
            // Handle millis() overflow
            unsigned long elapsed = currentMillis - manualOnTimestamp[i];

            if (elapsed >= MANUAL_TIMEOUT_MS) {
                // Timeout expired - switch to AUTO
                digitalWrite(RELAY_PINS[i], RELAY_OFF);
                outputStatus[i] = OUTPUT_AUTO;
                manualOnTimestamp[i] = 0;
                saveOutputStatus();

                Serial.printf("Zone %d: 30-minute timeout expired, switched to AUTO\n", i + 1);
            }
        }
    }
}