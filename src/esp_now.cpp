#include <wiz_mote.h>

#define DEST_ADDR ESPNOW_BROADCAST_ADDRESS

const unsigned long throttleInterval = 500;
unsigned long lastButtonPressTime = 0;

void espNowSentCB(uint8_t *address, uint8_t status)
{
    Serial.printf(PSTR("Message sent to " MACSTR ", status: %d\n"), MAC2STR(address), status);
}

void espNowReceiveCB(uint8_t *address, uint8_t *data, uint8_t len, signed int rssi, bool broadcast)
{
    // Check for throttling
    unsigned long currentMillis = millis();
    if (currentMillis - lastButtonPressTime < throttleInterval)
    {
        return;
    }
    lastButtonPressTime = currentMillis;

    sprintf_P(lastSignalSrc, PSTR("%02x%02x%02x%02x%02x%02x"), address[0], address[1], address[2], address[3], address[4], address[5]);
    if (data[0] == 0x91 || data[0] == 0x81 || data[0] == 0x80)
    {
        handleRemoteData(data, len, rssi);
    }
    else
    {
        Serial.printf("Unknown incoming ESP Now message received of length ");
        Serial.println(len);
    }
}

void setupESPNow()
{
    quickEspNow.onDataRcvd([](uint8_t *address, uint8_t *data, uint8_t len, signed int rssi, bool broadcast) {
        espNowReceiveCB(address, data, len, rssi, broadcast);
    });
    quickEspNow.onDataSent(espNowSentCB);
    quickEspNow.begin();
}
