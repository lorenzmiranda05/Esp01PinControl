#include <Arduino.h>
#include <Utility.h>

void setup()
{
  Serial.begin(921600);
  TelnetStream.begin();
  loadConfigFile();
  setupOTA();
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(relayPin, HIGH);
  server.on("/", handleOnConnect);
  server.on("/toggle", handleToggle);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop()
{
  if (wm.run() != WL_CONNECTED)
  {
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