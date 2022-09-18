#include "SensorDeLuz.h"
#include "SensorDHT.h"
#include "higrometro.h"
#include "Archivos.h"

class Controlador
{
private:
    // Objeto del higrómetro
    Higrometro* higrometro;
    // Objeto del sensor de luz
    SensorDeLuz* sensorDeLuz;
    // Objeto del sensor de humedad y temperatura del aire
    SensorDHT* dht;
    // Objeto del gestor de archivos del sistema
    Archivos* archivos;
public:
    Controlador(int AOut, int Dat);
    bool empezar(TwoWire *I2C = nullptr);
    float medirHumedad(bool forzar = false);
    float medirTemperatura(bool forzar = false);
    float calcularSensacionTermica();
    float medirLuz();
    int medirHumedadSuelo();
};