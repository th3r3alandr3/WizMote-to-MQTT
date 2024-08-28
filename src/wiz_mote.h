#ifndef WIZMOTE_H
#define WIZMOTE_H

#include <Arduino.h>

#if defined ESP32
#include <WiFi.h>
#include <esp_wifi.h>
#elif defined ESP8266
#include <ESP8266WiFi.h>
#define WIFI_MODE_STA WIFI_STA
#else
#error "Unsupported platform"
#endif // ESP32

#include <QuickEspNow.h>
#include <ArduinoJson.h>
#include <AsyncMqttClient.h>

#include <set>
#include <queue>
#include <string>

#include <func_declare.h>

// global variables
#ifndef WIZMOTE_DEFINE_GLOBAL_VARS
#define WIZMOTE_GLOBAL extern
#define _INIT(x)
#define _INIT_N(x)
#define _INIT_PROGMEM(x)
#else
#define WIZMOTE_GLOBAL
#define _INIT(x) = x
#define UNPACK(...) __VA_ARGS__
#define _INIT_N(x) UNPACK x
#define _INIT_PROGMEM(x) PROGMEM = x
#endif

// mqtt.cpp
WIZMOTE_GLOBAL AsyncMqttClient *mqttClient _INIT(NULL);
#define WIZMOTE_MQTT_CONNECTED (mqttClient != nullptr && mqttClient->connected())
WIZMOTE_GLOBAL WiFiClient wifiClient;
WIZMOTE_GLOBAL std::set<String> publishedDevices;
enum MessageType
{
  ACTION,
  BATTERY,
  AUTOMATION,
  LINK_QUALITY
};
struct Message {
    std::string action;
    std::string device;
    MessageType type;
};
WIZMOTE_GLOBAL std::queue<Message> messageQueue;
WIZMOTE_GLOBAL char mqttServer[MQTT_MAX_SERVER_LEN + 1] _INIT(MQTT_SERVER_IP);
WIZMOTE_GLOBAL uint16_t mqttPort _INIT(MQTT_SERVER_PORT);
WIZMOTE_GLOBAL char mqttUser[41] _INIT(MQTT_USER);
WIZMOTE_GLOBAL char mqttPass[65] _INIT(MQTT_PASSWORD);
WIZMOTE_GLOBAL char mqttClientID[41] _INIT(MQTT_CLIENT_ID);

// esp_now.cpp
WIZMOTE_GLOBAL char lastSignalSrc[13];
WIZMOTE_GLOBAL uint32_t lastSeq _INIT(UINT32_MAX);


class WizMote
{
public:
    WizMote();
    static WizMote &instance()
    {
        static WizMote instance;
        return instance;
    }

    void setup();
    void loop();
};

#endif // WIZMOTE_H