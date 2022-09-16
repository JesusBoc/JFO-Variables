#include "SensorDeLuz.h"
#include "SensorDHT.h"

class Controlador
{
private:
    // Pin del higrómetro
    int pinAOut;
    // Objeto del sensor de luz
    SensorDeLuz* sensorDeLuz;
    // Objeto del sensor de humedad y temperatura del aire
    SensorDHT* dht;
public:
    Controlador(int AOut, int Dat, TwoWire *I2C = nullptr);
    float medirHumedad(bool forzar = false);
};