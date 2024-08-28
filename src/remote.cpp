#include <wiz_mote.h>

#define WIZMOTE_BUTTON_ON 1
#define WIZMOTE_BUTTON_OFF 2
#define WIZMOTE_BUTTON_NIGHT 3
#define WIZMOTE_BUTTON_ONE 16
#define WIZMOTE_BUTTON_TWO 17
#define WIZMOTE_BUTTON_THREE 18
#define WIZMOTE_BUTTON_FOUR 19
#define WIZMOTE_BUTTON_BRIGHT_UP 9
#define WIZMOTE_BUTTON_BRIGHT_DOWN 8
#define WIZ_SMART_BUTTON_ON 100
#define WIZ_SMART_BUTTON_OFF 101
#define WIZ_SMART_BUTTON_BRIGHT_UP 102
#define WIZ_SMART_BUTTON_BRIGHT_DOWN 103

typedef struct WizMoteMessageStructure
{
    uint8_t program;  // 0x91 for ON button, 0x81 for all others
    uint8_t seq[4];   // Incremetal sequence number 32 bit unsigned integer LSB first
    uint8_t dt1;      // Button Data Type (0x32)
    uint8_t button;   // Identifies which button is being pressed
    uint8_t dt2;      // Battery Level Data Type (0x01)
    uint8_t batLevel; // Battery Level 0-100

    uint8_t byte10; // Unknown, maybe checksum
    uint8_t byte11; // Unknown, maybe checksum
    uint8_t byte12; // Unknown, maybe checksum
    uint8_t byte13; // Unknown, maybe checksum
} message_structure_t;

void handleRemoteData(uint8_t *incomingData, size_t len, signed int rssi)
{
    message_structure_t *incoming = reinterpret_cast<message_structure_t *>(incomingData);

    if (len != sizeof(message_structure_t))
    {
        Serial.printf("Unknown incoming ESP Now message received of length ");
        Serial.println(len);
        return;
    }

    uint32_t curSeq = incoming->seq[0] | (incoming->seq[1] << 8) | (incoming->seq[2] << 16) | (incoming->seq[3] << 24);

    // Publish discovery message if the device is not already published
    if (publishedDevices.find(lastSignalSrc) == publishedDevices.end())
    {
        messageQueue.push({"action", lastSignalSrc, MessageType::ACTION});
        messageQueue.push({"battery", lastSignalSrc, MessageType::BATTERY});
        messageQueue.push({"linkquality", lastSignalSrc, MessageType::LINK_QUALITY});
        messageQueue.push({"turn_on", lastSignalSrc, MessageType::AUTOMATION});
        messageQueue.push({"turn_off", lastSignalSrc, MessageType::AUTOMATION});
        messageQueue.push({"button_1", lastSignalSrc, MessageType::AUTOMATION});
        messageQueue.push({"button_2", lastSignalSrc, MessageType::AUTOMATION});
        messageQueue.push({"button_3", lastSignalSrc, MessageType::AUTOMATION});
        messageQueue.push({"button_4", lastSignalSrc, MessageType::AUTOMATION});
        messageQueue.push({"button_night", lastSignalSrc, MessageType::AUTOMATION});
        messageQueue.push({"button_up", lastSignalSrc, MessageType::AUTOMATION});
        messageQueue.push({"button_down", lastSignalSrc, MessageType::AUTOMATION});
        publishedDevices.insert(lastSignalSrc);
    }

    String message;
    switch (incoming->button)
    {
    case WIZ_SMART_BUTTON_ON:
    case WIZMOTE_BUTTON_ON:
        message = "turn_on";
        break;
    case WIZ_SMART_BUTTON_OFF:
    case WIZMOTE_BUTTON_OFF:
        message = "turn_off";
        break;
    case WIZMOTE_BUTTON_ONE:
        message = "button_1";
        break;
    case WIZMOTE_BUTTON_TWO:
        message = "button_2";
        break;
    case WIZMOTE_BUTTON_THREE:
        message = "button_3";
        break;
    case WIZMOTE_BUTTON_FOUR:
        message = "button_4";
        break;
    case WIZMOTE_BUTTON_NIGHT:
        message = "button_night";
        break;
    case WIZ_SMART_BUTTON_BRIGHT_UP:
    case WIZMOTE_BUTTON_BRIGHT_UP:
        message = "button_up";
        break;
    case WIZ_SMART_BUTTON_BRIGHT_DOWN:
    case WIZMOTE_BUTTON_BRIGHT_DOWN:
        message = "button_down";
        break;
    default:
        break;
    }

    if (message.length() == 0)
    {
        return;
    }

    JsonDocument doc;

    doc["action"] = message;
    doc["linkquality"] = rssi;
    doc["battery"] = incoming->batLevel;
    doc["operation_mode"] = "event";

    String payload;
    serializeJson(doc, payload);

    String actionTopic = String(MQTT_TOPIC) + "/" + lastSignalSrc + "/action";
    publishMqtt(actionTopic.c_str(), message.c_str(), true);

    String topic = String(MQTT_TOPIC) + "/" + lastSignalSrc;
    publishMqtt(topic.c_str(), payload.c_str(), true);

    doc["action"] = "";
    payload = "";
    serializeJson(doc, payload);
    publishMqtt(topic.c_str(), payload.c_str(), true);

    lastSeq = curSeq;
}
