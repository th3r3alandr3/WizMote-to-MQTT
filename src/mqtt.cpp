#include <wiz_mote.h>

#define MQTT_KEEP_ALIVE_TIME 60

void publishMqtt(const char *topic, const char *message, bool retain)
{
    if (!WIZMOTE_MQTT_CONNECTED)
        return;

    mqttClient->publish(topic, 0, retain, message);
}

static void onMqttConnect(bool sessionPresent)
{
    if (sessionPresent)
    {
        Serial.println("Session present");
    }
    publishMqtt("espnow2mqtt/bridge/state", "{\"state\": \"online\"}", true);
}

void onMqttError(AsyncMqttClientDisconnectReason reason)
{
    Serial.print("MQTT connection failed: ");
    switch (reason)
    {
    case AsyncMqttClientDisconnectReason::TCP_DISCONNECTED:
        Serial.println("TCP disconnected");
        break;
    case AsyncMqttClientDisconnectReason::MQTT_UNACCEPTABLE_PROTOCOL_VERSION:
        Serial.println("Unacceptable protocol version");
        break;
    case AsyncMqttClientDisconnectReason::MQTT_IDENTIFIER_REJECTED:
        Serial.println("Identifier rejected");
        break;
    case AsyncMqttClientDisconnectReason::MQTT_SERVER_UNAVAILABLE:
        Serial.println("Server unavailable");
        break;
    case AsyncMqttClientDisconnectReason::MQTT_MALFORMED_CREDENTIALS:
        Serial.println("Malformed credentials");
        break;
    case AsyncMqttClientDisconnectReason::MQTT_NOT_AUTHORIZED:
        Serial.println("Not authorized");
        break;
    default:
        Serial.println("Unknown reason");
        break;
    }

    // reconnect after 5 seconds
    delay(5000);
    mqttClient->connect();
}

void setupMqtt()
{
    Serial.println("Initializing MQTT...");
    if (mqttServer[0] == 0)
    {
        Serial.println("MQTT server not set");
        return;
    }

    if (mqttClient == nullptr)
    {
        mqttClient = new AsyncMqttClient();
        if (!mqttClient)
        {
            Serial.println("Failed to create MQTT client");
            return;
        }
        mqttClient->onConnect(onMqttConnect);
        mqttClient->onDisconnect(onMqttError);
    }
    else
    {
        Serial.println("MQTT client already exists");
    }
    if (mqttClient->connected())
    {
        Serial.println("MQTT already connected");
        return;
    }
    else
        Serial.println("MQTT not connected");

    IPAddress mqttIP;
    if (mqttIP.fromString(mqttServer)) // see if server is IP or domain
    {
        mqttClient->setServer(mqttIP, mqttPort);
    }
    else
    {
        mqttClient->setServer(mqttServer, mqttPort);
    }
    mqttClient->setClientId(mqttClientID);
    if (mqttUser[0] && mqttPass[0])
    {
        Serial.println("Setting MQTT credentials");
        mqttClient->setCredentials(mqttUser, mqttPass);
    }

    mqttClient->setKeepAlive(MQTT_KEEP_ALIVE_TIME);

    mqttClient->connect();
    Serial.println("MQTT initialized");
}


void loopMqtt()
{
    if (!messageQueue.empty())
    {
        auto message = messageQueue.front();
        switch (message.type)
        {
        case MessageType::ACTION:
            publishActionSensorDiscoveryMessage(String(message.device.c_str()));
            break;
        case MessageType::BATTERY:
            publishBatterySensorDiscoveryMessage(String(message.device.c_str()));
            break;
        case MessageType::LINK_QUALITY:
            publishLinkQualitySensorDiscoveryMessage(String(message.device.c_str()));
            break;
        case MessageType::AUTOMATION:
            publishAutomationDiscoveryMessage(String(message.device.c_str()), String(message.action.c_str()));
            break;
        default:
            break;
        }
        messageQueue.pop();
    }
}
