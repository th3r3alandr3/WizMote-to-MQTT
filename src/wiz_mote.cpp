#define WIZMOTE_DEFINE_GLOBAL_VARS // only in one source file wiz_mote.cpp
#include <wiz_mote.h>

WizMote::WizMote() {}

void WizMote::setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("Connected to %s in channel %d\n", WiFi.SSID().c_str(), WiFi.channel());
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("MAC address: %s\n", WiFi.macAddress().c_str());

    setupMqtt();
    setupESPNow();
}

void WizMote::loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.reconnect();
    }
    if (WIZMOTE_MQTT_CONNECTED)
    {
        loopMqtt();
    }
}