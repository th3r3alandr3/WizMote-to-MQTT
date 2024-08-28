#include <QuickEspNow.h>

// esp_now.cpp
void setupESPNow();
void espNowReceiveCB(uint8_t *incomingData, size_t len, signed int rssi);
void espNowSentCB(uint8_t *mac_addr, uint8_t status);

// homeassistant.cpp
void publishActionSensorDiscoveryMessage(String deviceId);
void publishBatterySensorDiscoveryMessage(String deviceId);
void publishAutomationDiscoveryMessage(String deviceId, String action);

// mqtt.cpp
void setupMqtt();
void loopMqtt();
void publishMqtt(const char *topic, const char *message, bool retain = false);

// remote.cpp
void handleRemoteData(uint8_t *incomingData, size_t len, signed int rssi);

