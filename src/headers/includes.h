#ifndef includes_h
#define includes_h

#define CONFIG_LITTLEFS_CACHE_SIZE 512

#include <Arduino.h>

#include "Update.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <LITTLEFS.h>
#include <Wire.h>
#include <Ticker.h>
#include "RTClib.h"
#include <rom/rtc.h>
#include <ESPmDNS.h>
#include <FastLED.h>
#include <DNSServer.h> // <-- for captive portal?

#include <headers/includes.h>
#include <headers/defines.h>
#include <headers/espTime.h>
#include <headers/fileSystem.h>
#include <headers/serverHandling.h>
#include <headers/wifiHandling.h>
#include <headers/light.h>
#include <headers/webSocket.h>
#include <headers/taskInits.h>

#endif