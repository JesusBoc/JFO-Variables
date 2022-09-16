#include "control.h"

Controlador::Controlador(int aout, int Dat, TwoWire *I2C){
    sensorDeLuz = new SensorDeLuz();
    dht = new SensorDHT(Dat);
    pinAOut = aout;
}

float Controlador::medirHumedad(bool forzar){
    return dht->leerHumedad(forzar);
}