//https://github.com/4-20ma/ModbusMaster/blob/master/src/ModbusMaster.cpp
#include "ModbusCommunication.h"

ModbusCommunication::ModbusCommunication(int slaveId, int dePin, int txdPin, int rxdPin, int serialBaud) {
  _slaveId = slaveId;
  _dePin = dePin;
  _txdPin = txdPin;
  _rxdPin = rxdPin;
  _serialBaud = serialBaud;
  _node = new ModbusMaster();
  _serial = new SoftwareSerial(_rxdPin, _txdPin);
}

bool ModbusCommunication::begin() {
  // Initialise la communication série pour la liaison RS485
  _serial->begin(_serialBaud);
  pinMode(_dePin, OUTPUT);
  digitalWrite(_dePin, LOW); // Désactiver transmition
  Serial.begin(_serialBaud);
  delay(10);

  // Attache la communication série à l'instance ModbusMaster
  _node->preTransmission(&(ModbusCommunication::preTransmission));
  _node->postTransmission(&(ModbusCommunication::postTransmission));
  _node->begin(_slaveId, *_serial);

  return true;
}

int ModbusCommunication::readHoldingRegisters(int address, int count) {
  int result = _node->readHoldingRegisters(address, count);
  if (result == _node->ku8MBSuccess) {
    Serial.println("Réception trame readHoldingRegisters OK !");
    Serial.println(_node->getResponseBuffer(0), HEX);
    return _node->getResponseBuffer(0);
  } else {
    Serial.print("Erreur sur trame readHoldingRegisters: ");
    Serial.println(result, HEX);
    return -1;
  }
}

void ModbusCommunication::preTransmission() {
  digitalWrite(/*ModbusCommunication::*/_dePin, HIGH); // Activer transmission
}

void ModbusCommunication::postTransmission() {
   digitalWrite(/*ModbusCommunication::*/_dePin, LOW); // Désactiver transmission
}
