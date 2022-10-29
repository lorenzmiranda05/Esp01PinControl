### **ESP-01 Relay Control via Website Button and Momentary Switch**

---
<br  />

**Features**
* Serial Monitor
* WiFi Connection
* Telnet Monitor
* OTA Updates
    * Firmware
    * Filesystem
* Website Hosting
---
<br  />

**data>config.json**
1. The config.json file looks like this:

    ```
    {
        "deviceType" : "ESP01",
        "broadcastDeviceDetails" : 0,
        "ipAddress" : [192, 168, 1, 71],
        "otaPassword" : "12345",
        "accessPoint" : [
                            {
                                "ssid": "WiFi1",
                                "password": "12345"
                            },
                            {
                                "ssid": "WiFi2",
                                "password": "12345"
                            },
                            {
                                "ssid": "WiFi3",
                                "password": "12345"
                            }
                        ]
    }
    ```

<br  />

---
<br  />

**Dependency Graph**

|-- ArduinoJson @ 6.19.4
<br  />
|-- TelnetStream @ 1.2.2
<br  />
|&nbsp; &nbsp; &nbsp;|-- ESP8266WiFi @ 1.0
<br  />
|&nbsp; &nbsp; &nbsp;|-- Ethernet @ 2.0.0
<br  />
|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|-- SPI @ 1.0
<br  />
|-- Utility
<br  />
|&nbsp; &nbsp; &nbsp;|-- ArduinoJson @ 6.19.4
<br  />
|&nbsp; &nbsp; &nbsp;|-- ArduinoOTA @ 1.0
<br  />
|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|-- ESP8266WiFi @ 1.0
<br  />
|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|-- ESP8266mDNS @ 1.2
<br  />
|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|-- ESP8266WiFi @ 1.0
<br  />
|&nbsp; &nbsp; &nbsp;|-- ESP8266WebServer @ 1.0
<br  />
|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|-- ESP8266WiFi @ 1.0
<br  />
|&nbsp; &nbsp; &nbsp;|-- ESP8266WiFi @ 1.0
<br  />
|&nbsp; &nbsp; &nbsp;|-- LittleFS @ 0.1.0
<br  />
|&nbsp; &nbsp; &nbsp;|-- TelnetStream @ 1.2.2
<br  />
|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|-- ESP8266WiFi @ 1.0
<br  />
|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|-- Ethernet @ 2.0.0
<br  />
|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|&nbsp; &nbsp; &nbsp;|-- SPI @ 1.0

<br  />

---
<br  />

**Web Site**
1. Landing Page

    ![github](https://raw.githubusercontent.com/lorenzmiranda05/Esp01PinControl/main/Assets/Images/01%20LED%20Bulb%20Control%20Landing%20Page.png)

1. Toggle Page

    ![github](https://raw.githubusercontent.com/lorenzmiranda05/Esp01PinControl/main/Assets/Images/02%20LED%20Bulb%20Control%20Toggle%20Page.png)

    ![github](https://raw.githubusercontent.com/lorenzmiranda05/Esp01PinControl/main/Assets/Images/03%20LED%20Bulb%20Control%20Toggle%20Page.png)

---
<br  />

**Tasks**
* [x] Connect to WiFi using ESP8266WiFiMulti.h
* [x] Use WiFi credentials from File System
* [x] Update embedded program using ArduinoOTA.h
* [x] Print to serial and telnet streams
* [x] Control the output of GPIO 0 via a website hosted with ESP8266WebServer.h
* [x] Control the output of GPIO 0 with a push button connected to GPIO 2
* [x] Allow button switch to function even without WiFi connection
* [x] Add static IP Address function
* [x] Load static IP Address from config.json file
* [x] Remove relay flicker on ESP start-up
* [x] Add OTA Password

<!-- Reusable and Invisible URL Definitions  -->
[Github Link]: https://github.com/lorenzmiranda05/Esp8266LiteTemplate

[BINARY UPDATES]: https://www.youtube.com/watch?v=pqaaPSRiYec

[BINARY UPDATES GITHUB]: https://github.com/binaryupdates/NodeMCU-Webserver-Station-Mode

[GPIO Input Wiring]: https://www.youtube.com/watch?v=wVJxG27fo8U

[Debounce Code]: https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce/