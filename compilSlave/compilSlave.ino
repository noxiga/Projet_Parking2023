#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>
//https://github.com/CMB27/ModbusRTUSlave/

const int redPin = 5;
const int greenPin = 2;
const int bluePin = 16;
const int triggerPin = 14;
const int echoPin = 12;
int distanceThreshold = 0;
int r = -1;

//////////////////////////////////////////////////////////

const byte rxPin = 13;
const byte txPin = 15;
const byte dePin = 0;//valide le driver de lignes de transmission
const byte id = 1;//adr slave?
const unsigned long baud = 9600;
const unsigned int bufSize = 256;
uint8_t buf[bufSize];//buffeur de réception des trames modbus. controlé par l'objet ModbusRTUSlave  
const unsigned int numHoldingRegisters = 2;//Nombre de registres
const unsigned int numCoils = 2;
unsigned long debut;

uint16_t holdingRegisters[numHoldingRegisters];
/*Role de chaque bit des registres à définir!
 holdingRegisters[0] : %fedcba9876543210
 holdingRegisters[1] : %fedcba9876543210
 */

SoftwareSerial comRS485(rxPin, txPin);

ModbusRTUSlave slaveModbus(comRS485,buf,bufSize,dePin);
//ModbusRTUSlave::ModbusRTUSlave(Stream& serial, uint8_t *buf, uint16_t bufSize, uint8_t dePin = NO_DE_PIN, uint32_t responseDelay = 0);
//uint16_t ModbusRTUSlave::_numHoldingRegisters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int measureBaseDistance() {
  long totalDistance = 0;
  const int numSamples = 10;

  for (int i = 0; i < numSamples; i++) {
    // Envoie une impulsion de déclenchement au capteur ultrason
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Mesure la durée de l'impulsion renvoyée par le capteur
    long duration = pulseIn(echoPin, HIGH);

    // Calcule la distance à l'objet en utilisant la vitesse du son
    long distance = duration * 0.034 / 2;

    totalDistance += distance;
    delay(50); // Attend un court instant entre chaque mesure
  }

  // Calcule la moyenne des distances mesurées
  int averageDistance = totalDistance / numSamples;
  Serial.print("Distance de base : ");
  Serial.print(averageDistance);
  Serial.println(" cm");

  return averageDistance;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t holdingRegisterRead(uint16_t address)//FC03
{
  long retour =-1;
  Serial.print("FC3 adr");
  Serial.println(address);
  if (address < numHoldingRegisters)
  {
    
    retour = holdingRegisters[address];
    //holdingRegisters[address]++;
  }
  return retour;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean holdingRegisterWrite(word address, word value)//FC06,FC16
{
  boolean retour=false;
  if (address < numHoldingRegisters)
  {
    holdingRegisters[address]=value;
    retour = true;
  }
  return retour;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int8_t coilRead(uint16_t address)
{
  char coil=0;
  //lire une broche?
  return coil;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean coilWrite(uint16_t address, boolean value)
{
  boolean retour=false;
  //affecter une broche?
  return retour;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(baud);
  Serial.println("Début du programme");
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  distanceThreshold = measureBaseDistance(); // Mesure la distance de base lors de la configuration initiale
  
  while(!Serial);
  Serial.print("Esclave RTU ");
  Serial.println(id);
  
  comRS485.begin(baud);
  slaveModbus.begin(id, baud);//void ModbusRTUSlave::begin(uint8_t id, uint32_t baud, uint8_t config)
  
  //void ModbusRTUSlave::configureCoils(uint16_t numCoils, BoolRead coilRead, BoolWrite coilWrite)
  slaveModbus.configureCoils(numCoils, coilRead, coilWrite);
  //void ModbusRTUSlave::configureDiscreteInputs(uint16_t numDiscreteInputs, BoolRead discreteInputRead)
  //void ModbusRTUSlave::configureHoldingRegisters(uint16_t numHoldingRegisters, WordRead holdingRegisterRead, WordWrite holdingRegisterWrite)
  slaveModbus.configureHoldingRegisters(numHoldingRegisters, holdingRegisterRead, holdingRegisterWrite);
  //void ModbusRTUSlave::configureInputRegisters(uint16_t numInputRegisters, WordRead inputRegisterRead)
  debut = millis();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // Envoie une impulsion de déclenchement au capteur ultrason
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Mesure la durée de l'impulsion renvoyée par le capteur
  long duration = pulseIn(echoPin, HIGH);

  // Calcule la distance à l'objet en utilisant la vitesse du son
  float distance = (duration * 0.0343) / 2;
  
  Serial.print(distance);
  Serial.print(" : ");

  if (distance < distanceThreshold) {
    // allume la LED en rouge si la distance est inférieure à la distance seuil
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    Serial.println("Rouge");
    r = 1;
  } 
  else {
    // allume la LED en vert si la distance est supérieure à la distance seuil
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    Serial.println("Vert");
    r = 0;
  }
  
  slaveModbus.poll();//une trame modbus? alors traite la.
  //lire le capteur de présense et actualiser le registre holdingRegisters[0];
  if (millis()-debut > 1000){
    holdingRegisters[0]= r ;
    Serial.print("Valeur : ");
    Serial.println(holdingRegisters[0]);
    debut = millis();
  }
  
  delay(1000);
}
