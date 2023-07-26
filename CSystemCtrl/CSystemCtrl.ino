#include "CPub.h"
#include "ModbusCommunication.h"

#define DE_PIN 0
#define TXD_PIN 15
#define RXD_PIN 13
#define SERIAL_BAUD 9600
#define MSG_BUFFER_SIZE (50)

const char* ssid = "SNUFA";
const char* password = "guigui59";
const char* mqtt_server = "172.16.150.57";

ModbusMaster* _node = nullptr;

int sl =1;
ModbusCommunication modbus(sl, DE_PIN, TXD_PIN, RXD_PIN, SERIAL_BAUD);
CPub pub;

void setup() {
  Serial.begin(SERIAL_BAUD);
  _node = new ModbusMaster();
  modbus.begin();
  pub.setup(ssid, password, mqtt_server);
}

void loop() {
  modbus.readHoldingRegisters(0, 1);
  char msg[MSG_BUFFER_SIZE];
  String variable2 = "erreur";
  uint16_t responseSize = _node->getResponseBuffer(0);
  bool isOnePresent = false;
  
  if (responseSize <= MSG_BUFFER_SIZE) {
    //Serial.print("responseSize :");
    //Serial.println(responseSize);
    for (uint16_t i = 0; i < responseSize; i++) {
      
      msg[i] = _node->getResponseBuffer(i);
      if (msg[i] == '1') {
        isOnePresent = true;
        Serial.println("test true");
      } else {
        isOnePresent = false;
        Serial.println("test false");
      }
    }

    if (isOnePresent == true) {
      variable2 = "prise";
      Serial.println("prise");
    } else {
      variable2 = "libre";
      Serial.println("libre");
    }
    
    const char* charArray = variable2.c_str();
    pub.loop();
    pub.publish("etage1", charArray);
    Serial.println("......");
  } else {
    Serial.println("Taille du message dépasse la taille du tampon de message !");
    Serial.println("......");
  }
  
  delay(5000);
}
