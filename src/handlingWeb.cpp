// 
// 
// 

#include "handlingWeb.h"
#include "html.h"

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <DNSServer.h>

#include <IotWebConfAsyncUpdateServer.h>
#include <IotWebConfAsync.h>
#include <IotWebRoot.h>
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
#define STATUS_PIN LED_BUILTIN
#define ON_LEVEL HIGH

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "GardenaController";

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebServerWrapper asyncWebServerWrapper(&server);
AsyncUpdateServer AsyncUpdater;

NtpConfig ntpConfig = NtpConfig();

AsyncIotWebConf iotWebConf(thingName, &dnsServer, &asyncWebServerWrapper, wifiInitialApPassword, CONFIG_VERSION);

OutputStatus outputStatus[MAX_SCHEDULERS]{
    OUTPUT_OFF,
    OUTPUT_OFF,
    OUTPUT_OFF,
    OUTPUT_OFF,
    OUTPUT_OFF,
    OUTPUT_OFF,
    OUTPUT_OFF,
    OUTPUT_OFF
};

bool ShouldReboot = false;
bool ConfigChanged = false;

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

class CustomHtmlFormatProvider : public iotwebconf::OptionalGroupHtmlFormatProvider {
protected:
    String getFormEnd() override {
        return OptionalGroupHtmlFormatProvider::getFormEnd() + String(FPSTR(html_form_end));
    }
    String getEnd() override {
        return String(FPSTR(html_end_template)) + HtmlFormatProvider::getEnd();

    }
};
CustomHtmlFormatProvider customHtmlFormatProvider;

class MyHtmlRootFormatProvider : public HtmlRootFormatProvider {
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
        // BRIGHTNESS-FELD KOMPLETT ENTFERNT FÜR GARDENA CONTROLLER!
        html_ += "</div></td></tr>";
        return html_;
    }

protected:
    String getStyleInner() override {
        String s_ = HtmlRootFormatProvider::getStyleInner();
        s_ += String(FPSTR(html_output_styles));
        return s_;
    }

    String getScriptInner() override {
        String s_ = HtmlRootFormatProvider::getScriptInner();

        s_.replace("{millisecond}", "5000");
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
    pref_.begin("outputStatus", true);

    for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
        String key_ = "ch" + String(i_ + 1);
        if (pref_.isKey(key_.c_str())) {
            outputStatus[i_] = static_cast<OutputStatus>(pref_.getUChar(key_.c_str(), static_cast<uint8_t>(OUTPUT_AUTO)));
        }
    }

    pref_.end();
	Serial.println(F("Output status loaded from preferences."));
}


void setupWebServer() {

	loadOutputStatus();

    iotWebConf.setStatusPin(STATUS_PIN, ON_LEVEL);
    iotWebConf.setConfigPin(CONFIG_PIN);
    
    iotWebConf.setHtmlFormatProvider(&customHtmlFormatProvider);

    iotWebConf.addParameterGroup(&ntpConfig);
    iotWebConf.addParameterGroup(&soilMoistureSensor);

    for (int i_ = 0; i_ < MAX_SCHEDULERS; ++i_) {
        iotWebConf.addParameterGroup(&schedulers[i_]);
    }

    iotWebConf.getApTimeoutParameter()->visible = true;

    iotWebConf.setConfigSavedCallback(&handleConfigSaved);
    iotWebConf.setConfigSavedPage(&handleConfigSavedPage);

    iotWebConf.setConfigAPPasswordMissingPage(&handleAPPasswordMissingPage);
    iotWebConf.setConfigSSIDNotConfiguredPage(&handleSSIDNotConfiguredPage);

    iotWebConf.setWifiConnectionCallback(&handleWificonnected);
    iotWebConf.setWifiConnectionHandler(&handleConnectWifi);
    iotWebConf.setWifiConnectionFailedHandler(&handleConnectWifiFailure);

    iotWebConf.init();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleRoot(request);
        }
    );
    server.on("/config", HTTP_ANY, [](AsyncWebServerRequest* request) {
        auto* asyncWebRequestWrapper_ = new AsyncWebRequestWrapper(request);
        iotWebConf.handleConfig(asyncWebRequestWrapper_);
        }
    );
    server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest* request) {
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
    );

    server.on("/DateTime", HTTP_GET, [](AsyncWebServerRequest* request) { handleDateTime(request); });
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest* request) { handleData(request); });
    server.on("/post", HTTP_ANY, [](AsyncWebServerRequest* request) {
        handlePost(request);
        }
    );
    server.onNotFound([](AsyncWebServerRequest* request) {
        AsyncWebRequestWrapper asyncWebRequestWrapper_(request);
        iotWebConf.handleNotFound(&asyncWebRequestWrapper_);
        }
    );

}

void loopWebServer() {
    iotWebConf.doLoop();
}


void handleRoot(AsyncWebServerRequest* request) {
    AsyncWebRequestWrapper asyncWebRequestWrapper(request);
    if (iotWebConf.handleCaptivePortal(&asyncWebRequestWrapper)) {
        return;
    }

    MyHtmlRootFormatProvider fp_;

    String response_ = "";
    response_ += fp_.getHtmlHead(iotWebConf.getThingName());

    response_ += F("<link rel=\"icon\" type=\"image/png\" sizes=\"96x96\" href=\"/apple-touch-icon.png\">\n");
    response_ += F("<link rel=\"apple-touch-icon\" sizes=\"96x96\" href=\"/apple-touch-icon.png\">\n");

    response_ += fp_.getHtmlStyle();
    response_ += fp_.getHtmlHeadEnd();
    response_ += fp_.getHtmlScript();

    response_ += fp_.getHtmlTable();
    response_ += fp_.getHtmlTableRow() + fp_.getHtmlTableCol();

    // Status-Zeile mit DateTime und RSSI
    response_ += F("<fieldset align=left style=\"border: 1px solid\">\n");
    response_ += F("<table border=\"0\" align=\"center\" width=\"100%\">\n");
    response_ += F("<tr><td align=\"left\"><span id=\"DateTimeValue\">not valid</span></td><td align=\"right\"><span id=\"RSSIValue\">-100</span> dBm</td></tr>\n");
    response_ += fp_.getHtmlTableEnd();
    response_ += fp_.getHtmlFieldsetEnd();

	// Bodenfeuchte-Sensoren
    if (soilMoistureSensor.isEnabled()) {
        response_ += F("<fieldset align=left style=\"border: 1px solid\">\n");
        response_ += F("<legend>Soil Moisture Sensor</legend>\n");
        response_ += F("<table border=\"0\" align=\"center\" width=\"100%\">\n");

        String sensorName = String(soilMoistureSensor.getName());
        if (sensorName.length() == 0) {
            sensorName = "Soil Sensor";
        }
        response_ += fp_.getHtmlTableRowSpan(sensorName, "N/A", "SoilMoisturePercent");

        response_ += fp_.getHtmlTableEnd();
        response_ += fp_.getHtmlFieldsetEnd();
    }

    // Aktuelle Werte der Zonen
    response_ += F("<fieldset align=left style=\"border: 1px solid\">\n");
    response_ += F("<legend>Zonen Status</legend>\n");
    response_ += F("<table border=\"0\" align=\"center\" width=\"100%\">\n");

    for (uint8_t i = 0; i < MAX_SCHEDULERS; i++) {
        String zoneName = String(schedulers[i].getZone());
        if (zoneName.length() == 0) {
            zoneName = "Zone " + String(i + 1);
        }
        response_ += fp_.getHtmlTableRowSpan(zoneName, "OFF", "Zone" + String(i + 1) + "Status");
    }

    response_ += fp_.getHtmlTableEnd();
    response_ += fp_.getHtmlFieldsetEnd();

    // Manuelle Steuerung der Ausgänge
    response_ += F("<fieldset align=left style=\"border: 1px solid\">\n");
    response_ += F("<legend>Manuelle Steuerung</legend>\n");
    response_ += F("<table border=\"0\" align=\"center\" width=\"100%\">\n");

    for (uint8_t ch_ = 0; ch_ < MAX_SCHEDULERS; ++ch_) {
        String zoneName = String(schedulers[ch_].getZone());
        if (zoneName.length() == 0) {
            zoneName = "Zone " + String(ch_ + 1);
        }
        response_ += fp_.getOutputControlRow(zoneName, String(ch_ + 1), outputStatus[ch_], 0, false);
    }

    response_ += fp_.getHtmlTableEnd();
    response_ += fp_.getHtmlFieldsetEnd();

    response_ += fp_.addNewLine(2);

    // Footer mit Links
    response_ += fp_.getHtmlTable();
    response_ += fp_.getHtmlTableRowText("Open <a href='/config'>configuration page</a>.");
    response_ += fp_.getHtmlTableRowText("Open <a href='/webserial' target='_blank'>WebSerial Console</a> for live logs.");
    response_ += fp_.getHtmlTableRowText(fp_.getHtmlVersion(Version));
    response_ += fp_.getHtmlTableEnd();

    response_ += fp_.getHtmlTableColEnd() + fp_.getHtmlTableRowEnd();
    response_ += fp_.getHtmlTableEnd();
    response_ += fp_.getHtmlEnd();

    request->send(200, "text/html; charset=UTF-8", response_);
}

void handleDateTime(AsyncWebServerRequest* request) {
    ESP_LOGD("handleDateTime", "Time requested");

    // Get the current time
    time_t now_ = time(nullptr);
    struct tm* timeinfo_ = localtime(&now_);
    char timeStr_[20];
    char dateStr_[20];
    strftime(timeStr_, sizeof(timeStr_), "%H:%M:%S", timeinfo_);
    strftime(dateStr_, sizeof(dateStr_), "%Y-%m-%d", timeinfo_);

    request->send(200, "text/plain", timeStr_);
}

void handleData(AsyncWebServerRequest* request) {
    // JSON mit aktuellen Werten für AJAX-Updates
    String json_ = "{";
    json_ += "\"RSSI\":\"" + String(WiFi.RSSI()) + "\"";

    for (uint8_t i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
        int pinState_ = digitalRead(RELAY_PINS[i_]);
        String status_ = (pinState_ == LOW) ? "ON" : "OFF";
        json_ += ",\"Zone" + String(i_ + 1) + "Status\":\"" + status_ + "\"";
	}

    if (soilMoistureSensor.isEnabled()) {
        int soilMoisturePercent_ = soilMoistureSensor.getMoisturePercent();
        json_ += ",\"SoilMoisturePercent\":\"" + String(soilMoisturePercent_) + "\"";
	}

    json_ += "}";

    request->send(200, "application/json", json_);
}

void handlePost(AsyncWebServerRequest* request) {
    int params_ = request->params();

    if (request->hasParam("reset", true)) {

		ntpConfig.applyDefaultValue();
		soilMoistureSensor.applyDefaultValue();

        for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
            schedulers[i_].applyDefaultValue();
		}

        Serial.println("Resetting output status to AUTO");
        for (int i_ = 0; i_ < MAX_SCHEDULERS; i_++) {
            outputStatus[i_] = OUTPUT_AUTO;
            digitalWrite(RELAY_PINS[i_], HIGH); // Set all relays to OFF
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
                if (name_  == "ch" + String(z_)) {
                    int channel_ = z_ - 1;
                    if (value_ == "on") {
                        digitalWrite(RELAY_PINS[channel_], LOW);
                        Serial.printf("Zone %d turned ON\n", z_);
						Serial.printf("Relay pin %d set to LOW\n", RELAY_PINS[channel_]);
						outputStatus[channel_] = OUTPUT_ON;
                    }
                    else if (value_ == "off") {
                        digitalWrite(RELAY_PINS[channel_], HIGH);
                        Serial.printf("Zone %d turned OFF\n", z_);
						Serial.printf("Relay pin %d set to HIGH\n", RELAY_PINS[channel_]);
						outputStatus[channel_] = OUTPUT_OFF;
                    }
                    else if (value_ == "auto") {
                        Serial.printf("Zone %d set to AUTO\n", z_);
						outputStatus[channel_] = OUTPUT_AUTO;
                    }
                }
            }
			saveOutputStatus();
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