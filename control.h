#include "SensorDeLuz.h"
#include "SensorDHT.h"

class Controlador
{
private:
    // Pin del higrómetro
    int pinAOut;
    // Pin del DHT11
    int pinDat;
    // Pin del SPI para la tarjeta SD
    int pinSPI;
    // Objeto del sensor de luz
    SensorDeLuz* sensorDeLuz;
    // Objeto del sensor de humedad y temperatura del aire
    SensorDHT* dht;
public:
    void establecerBaudRate(int baudRate);
    Controlador(int AOut, int Dat, int pinSPI, TwoWire *I2C = nullptr);
};