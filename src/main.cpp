#include <Arduino.h>
#include <Utility.h>

void setup()
{
  Serial.begin(921600);
  TelnetStream.begin();
  delay(10);
  loadConfigFile();
  setupOTA();
}

void loop()
{
  ArduinoOTA.handle();
  if (wm.run() != WL_CONNECTED)
  {
    serialAndTelnetPrintln("WiFi not connected!");
    delay(5000);
  }
  if (wm.run() == WL_CONNECTED)
  {
    serialAndTelnetPrintln("");
    serialAndTelnetPrint("Connected to ");
    serialAndTelnetPrintln(WiFi.SSID());
    serialAndTelnetPrint("Device Name ");
    serialAndTelnetPrintln(espName);
    serialAndTelnetPrint("IP Address ");
    serialAndTelnetPrintln(WiFi.localIP());
    delay(5000);
  }
}