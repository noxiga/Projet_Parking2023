#include "CPub.h"
#include "ModbusCommunication.h"

CPub::CPub() : mqttClient(wifiClient) {}

void CPub::setup(const char* ssid, const char* password, const char* mqtt_server) {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  mqttClient.setServer(mqtt_server, 1883);
}

void CPub::loop() {
  if (!mqttClient.connected()) {
    Serial.println("Connecté !");
    if (mqttClient.connect("ESP8266Client")) {
      mqttClient.subscribe("etage1");
    }
  }
  mqttClient.loop();
}

void CPub::publish(const char* topic, const char* message) {
  mqttClient.publish(topic, message);
}
