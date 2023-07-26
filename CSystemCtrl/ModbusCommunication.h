#ifndef MODBUS_COMMUNICATION_H
#define MODBUS_COMMUNICATION_H

#include <ModbusMaster.h>
#include <SoftwareSerial.h>

static int _dePin;

class ModbusCommunication {
  public:
    ModbusCommunication(int slaveId, int dePin, int txdPin, int rxdPin, int serialBaud);
    bool begin();
    int readHoldingRegisters(int address, int count);
  private:
    int _slaveId;
    int _txdPin;
    int _rxdPin;
    int _serialBaud;
    ModbusMaster* _node;
    SoftwareSerial* _serial;
    static void preTransmission();
    static void postTransmission();
};
#endif
