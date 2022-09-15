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
    ArduinoOTA.handle();
    if (broadcastDeviceDetails == 0)
    {
      server.handleClient();
      handlePushButtonWithDebounce();
      if (pinStatus)
      {
        digitalWrite(relayPin, HIGH);
      }
      else
      {
        digitalWrite(relayPin, LOW);
      }
    }
    else
    {
      serialAndTelnetPrintln("");
      serialAndTelnetPrint("Device Name: ");
      serialAndTelnetPrintln(espName);
      serialAndTelnetPrint("WiFi Connection: ");
      serialAndTelnetPrintln(WiFi.SSID());
      serialAndTelnetPrint("MAC Address: ");
      serialAndTelnetPrintln(WiFi.macAddress());
      serialAndTelnetPrint("IP Address: ");
      serialAndTelnetPrintln(WiFi.localIP());
      serialAndTelnetPrintln("Web Server: OFF");
      delay(5000);
    }
  }
}