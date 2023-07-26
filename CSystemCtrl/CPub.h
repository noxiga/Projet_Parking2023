//https://github.com/knolleary/pubsubclient
#ifndef CPub_h
#define CPub_h

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

class CPub {
  public:
    CPub();
    void setup(const char* ssid, const char* password, const char* mqtt_server);
    void loop();
    void publish(const char* topic, const char* message);
  private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
};

#endif
