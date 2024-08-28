#include <wiz_mote.h>

void publishActionSensorDiscoveryMessage(String deviceId)
{
  String topic = String(MQTT_DISCOVERY_TOPIC_SENSOR) + "/" + deviceId + "/action/config";

  JsonDocument doc;

  JsonArray availability = doc["availability"].to<JsonArray>();
  JsonObject availability_0 = availability.add<JsonObject>();

  availability_0["topic"] = "espnow2mqtt/bridge/state";
  availability_0["value_template"] = "{{ value_json.state }}";

  JsonObject device = doc["device"].to<JsonObject>();
  device["identifiers"][0] = deviceId;
  device["manufacturer"] = "WiZ";
  device["model"] = "WizMote";
  device["sw_version"] = "1.0";
  device["name"] = "WizMote " + deviceId;

  doc["enabled_by_default"] = true;
  doc["icon"] = "mdi:gesture-double-tap";
  doc["object_id"] = deviceId + "_action";
  doc["state_topic"] = String(MQTT_TOPIC) + "/" + deviceId;
  doc["unique_id"] = deviceId;
  doc["value_template"] = "{{ value_json.action }}";
  doc["name"] = "Action";
  doc["platform"] = "mqtt";

  String payload;
  serializeJson(doc, payload);

  publishMqtt(topic.c_str(), payload.c_str(), true);
}

void publishBatterySensorDiscoveryMessage(String deviceId)
{
  String topic = String(MQTT_DISCOVERY_TOPIC_SENSOR) + "/" + deviceId + "/battery/config";

  JsonDocument doc;

  JsonArray availability = doc["availability"].to<JsonArray>();
  JsonObject availability_0 = availability.add<JsonObject>();

  availability_0["topic"] = "espnow2mqtt/bridge/state";
  availability_0["value_template"] = "{{ value_json.state }}";

  JsonObject device = doc["device"].to<JsonObject>();
  device["identifiers"][0] = deviceId;
  device["manufacturer"] = "WiZ";
  device["model"] = "WizMote";
  device["sw_version"] = "1.0";
  device["name"] = "WizMote " + deviceId;

  doc["device_class"] = "battery";
  doc["entity_category"] = "diagnostic";
  doc["enabled_by_default"] = true;
  doc["state_class"] = "measurement";
  doc["object_id"] = deviceId + "_battery";
  doc["state_topic"] = String(MQTT_TOPIC) + "/" + deviceId;
  doc["unique_id"] = deviceId + "_battery";
  doc["unit_of_measurement"] = "%";
  doc["value_template"] = "{{ value_json.battery }}";
  doc["platform"] = "mqtt";

  String payload;
  serializeJson(doc, payload);

  publishMqtt(topic.c_str(), payload.c_str(), true);
}

void publishLinkQualitySensorDiscoveryMessage(String deviceId)
{
  String topic = String(MQTT_DISCOVERY_TOPIC_SENSOR) + "/" + deviceId + "/linkquality/config";

  JsonDocument doc;

  JsonArray availability = doc["availability"].to<JsonArray>();
  JsonObject availability_0 = availability.add<JsonObject>();

  availability_0["topic"] = "espnow2mqtt/bridge/state";
  availability_0["value_template"] = "{{ value_json.state }}";

  JsonObject device = doc["device"].to<JsonObject>();
  device["identifiers"][0] = deviceId;
  device["manufacturer"] = "WiZ";
  device["model"] = "WizMote";
  device["sw_version"] = "1.0";
  device["name"] = "WizMote " + deviceId;

  doc["device_class"] = "signal_strength";
  doc["entity_category"] = "diagnostic";
  doc["enabled_by_default"] = false;
  doc["state_class"] = "measurement";
  doc["object_id"] = deviceId + "_linkquality";
  doc["state_topic"] = String(MQTT_TOPIC) + "/" + deviceId;
  doc["unique_id"] = deviceId + "_linkquality";
  doc["unit_of_measurement"] = "dBm";
  doc["value_template"] = "{{ value_json.linkquality }}";
  doc["platform"] = "mqtt";

  String payload;
  serializeJson(doc, payload);

  publishMqtt(topic.c_str(), payload.c_str(), true);
}

void publishAutomationDiscoveryMessage(String deviceId, String action)
{
  String topic = String(MQTT_DISCOVERY_TOPIC_AUTOMATION) + "/" + deviceId + "/action_" + action + "/config";

  JsonDocument doc;

  doc["automation_type"] = "trigger";

  JsonObject device = doc["device"].to<JsonObject>();
  device["identifiers"][0] = deviceId;
  device["manufacturer"] = "WiZ";
  device["model"] = "WizMote";
  device["sw_version"] = "1.0";
  device["name"] = "WizMote " + deviceId;

  JsonObject origin = doc["origin"].to<JsonObject>();
  origin["sw_version"] = "1.0";
  origin["name"] = "WizMote to MQTT";

  doc["payload"] = action;
  doc["subtype"] = action;
  doc["topic"] = String(MQTT_TOPIC) + "/" + deviceId + "/action";
  doc["type"] = "action";
  doc["platform"] = "mqtt";

  String payload;
  serializeJson(doc, payload);

  publishMqtt(topic.c_str(), payload.c_str(), true);
}
