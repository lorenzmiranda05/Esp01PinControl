#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <TelnetStream.h>
#include <ESP8266WebServer.h>

#define JsonConfigFile "/config.json"

ESP8266WiFiMulti WiFiMulti;
unsigned long previousMillis = 0;
unsigned long interval = 60000;
char espName[15];
int broadcastDeviceDetails = 1;
bool pinStatus = LOW;
ESP8266WebServer server(80);
const int buttonPin = 2;
const int relayPin = 0;

int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void serialAndTelnetPrint(__FlashStringHelper *message)
{
    Serial.print(message);
    TelnetStream.print(message);
}
void serialAndTelnetPrint(const char *message)
{
    Serial.print(message);
    TelnetStream.print(message);
}
void serialAndTelnetPrint(int message)
{
    Serial.print(message);
    TelnetStream.print(message);
}
void serialAndTelnetPrint(IPAddress message)
{
    Serial.print(message);
    TelnetStream.print(message);
}
void serialAndTelnetPrint(String message)
{
    Serial.print(message);
    TelnetStream.print(message);
}

void serialAndTelnetPrintln(__FlashStringHelper *message)
{
    Serial.println(message);
    TelnetStream.println(message);
}
void serialAndTelnetPrintln(const char *message)
{
    Serial.println(message);
    TelnetStream.println(message);
}
void serialAndTelnetPrintln(int message)
{
    Serial.println(message);
    TelnetStream.println(message);
}
void serialAndTelnetPrintln(IPAddress message)
{
    Serial.println(message);
    TelnetStream.println(message);
}
void serialAndTelnetPrintln(String message)
{
    Serial.println(message);
    TelnetStream.println(message);
}

bool loadConfigFile()
// Load existing configuration file
{
    // Uncomment if we need to format filesystem
    // LittleFS.format();

    // Read configuration from FS json
    serialAndTelnetPrintln(F("Mounting FS"));

    // May need to make it begin(true) first time you are using SPIFFS
    if (LittleFS.begin())
    {
        // The file exists, reading and loading
        serialAndTelnetPrintln(F("Reading config"));
        File configFile = LittleFS.open(JsonConfigFile, "r");
        if (configFile)
        {
            serialAndTelnetPrintln(F("Opened config"));
            StaticJsonDocument<512> json;
            DeserializationError error = deserializeJson(json, configFile);
            //  serializeJsonPretty(json, Serial);
            //  serializeJsonPretty(json, TelnetStream);
            if (!error)
            {
                // serialAndTelnetPrintln(F(""));
                serialAndTelnetPrintln(F("Parsing config"));
                strcpy(espName, json["deviceType"]);
                broadcastDeviceDetails = json["broadcastDeviceDetails"].as<int>();
                WiFiMulti.addAP(json["accessPoint"][0]["ssid"], json["accessPoint"][0]["password"]);
                WiFiMulti.addAP(json["accessPoint"][1]["ssid"], json["accessPoint"][1]["password"]);
                WiFiMulti.addAP(json["accessPoint"][2]["ssid"], json["accessPoint"][2]["password"]);
                IPAddress gateway(192, 168, 1, 1);
                IPAddress subnet(255, 255, 0, 0);
                IPAddress local_IP(json["ipAddress"][0].as<int>(), json["ipAddress"][1].as<int>(), json["ipAddress"][2].as<int>(), json["ipAddress"][3].as<int>());
                WiFi.config(local_IP, gateway, subnet);
                return true;
            }
            else
            {
                // Error loading JSON data
                serialAndTelnetPrintln(F("Load config failed"));
            }
        }
    }
    else
    {
        // Error mounting file system
        serialAndTelnetPrintln(F("Mount FS failed"));
    }
    return false;
}

void wifiReconnect()
{
    unsigned long currentMillis = millis();
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
    {
        WiFiMulti.run();
        previousMillis = currentMillis;
    }
}

void setupOTA()
{
    // Create unique ota host name by appending MAC Address
    uint16_t maxlen = strlen(espName) + 8;
    char *deviceName = new char[maxlen];
    uint8_t mac[6];
    WiFi.macAddress(mac);
    snprintf(deviceName, maxlen, "%s-%02x%02x%02x", espName, mac[3], mac[4], mac[5]);
    strcpy(espName, deviceName);
    ArduinoOTA.setHostname(deviceName);
    delete[] deviceName;

    ArduinoOTA.onStart([]()
                       {
                            serialAndTelnetPrint(F(""));
                            serialAndTelnetPrintln(F("Update start")); });

    ArduinoOTA.onEnd([]()
                     { serialAndTelnetPrintln(F("\nEnd")); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });

    ArduinoOTA.onError([](ota_error_t error)
                       {
                            Serial.printf("Error[%u]: ", error);
                            if (error == OTA_AUTH_ERROR) serialAndTelnetPrintln(F("\nAuth Failed"));
                            else if (error == OTA_BEGIN_ERROR) serialAndTelnetPrintln(F("\nBegin Failed"));
                            else if (error == OTA_CONNECT_ERROR) serialAndTelnetPrintln(F("\nConnect Failed"));
                            else if (error == OTA_RECEIVE_ERROR) serialAndTelnetPrintln(F("\nReceive Failed"));
                            else if (error == OTA_END_ERROR) serialAndTelnetPrintln(F("\nEnd Failed")); });

    ArduinoOTA.begin();
    serialAndTelnetPrintln(F("ESPOTA READY"));
}

String updateWebpage(uint8_t ledStatus)
{
    String ptr = "<!DOCTYPE html> <html>\n";
    ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<title>LED Bulb Control</title>\n";
    ptr += "<style>html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr += ".button {display: block;width: 100px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    ptr += ".button-on {background-color: #3498db;}\n";
    ptr += ".button-on:active {background-color: #34495e;}\n";
    ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr += "</style>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<h2>LED Bulb</h2>\n";
    ptr += "<h3>Location: 2F Bedroom</h3>\n";

    if (ledStatus)
    {
        ptr += "<p>State: OFF</p>\n";
    }
    else
    {
        ptr += "<p>State: ON</p>\n";
    }

    ptr += "<a class=\"button button-toggle\" href=\"/toggle\">TOGGLE</a>\n";
    ptr += "</body>\n";
    ptr += "</html>\n";
    return ptr;
}

void onMessage()
{
    serialAndTelnetPrintln("{\"Device\":\"LED Bulb\",\"Location\":\"2F Bedroom\",\"State\":\"ON\"}");
}

void offMessage()
{
    serialAndTelnetPrintln("{\"Device\":\"LED Bulb\",\"Location\":\"2F Bedroom\",\"State\":\"OFF\"}");
}

void handleOnConnect()
{
    pinStatus = digitalRead(relayPin);
    if (pinStatus)
    {
        offMessage();
    }
    else
    {
        onMessage();
    }
    server.send(200, "text/html", updateWebpage(pinStatus));
}

void handleToggle()
{
    if (digitalRead(relayPin) == 0)
    {
        pinStatus = 1;
        offMessage();
    }
    else if (digitalRead(relayPin) == 1)
    {
        pinStatus = 0;
        onMessage();
    }
    server.send(200, "text/html", updateWebpage(pinStatus));
}

void handleNotFound()
{
    server.send(404, "text/html", "404 - Page Not found");
}

void handlePushButtonWithDebounce()
{
    int reading = digitalRead(buttonPin);

    if (reading != lastButtonState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (reading != buttonState)
        {
            buttonState = reading;

            if (buttonState == LOW)
            {
                pinStatus = !pinStatus;
                if (pinStatus)
                {
                    offMessage();
                }
                else
                {
                    onMessage();
                }
            }
        }
    }
    lastButtonState = reading;
}