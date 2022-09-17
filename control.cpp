#include "control.h"

Controlador::Controlador(int aout, int Dat){
    sensorDeLuz = new SensorDeLuz();
    dht = new SensorDHT(Dat);
    higrometro = new Higrometro(aout);
}

float Controlador::medirHumedad(bool forzar){
    return dht->leerHumedad(forzar);
}

float Controlador::medirTemperatura(bool forzar){
    return dht->leerTemperatura();
}

float Controlador::calcularSensacionTermica(){
    return dht->calcularSensacionTermica();
}

float Controlador::medirLuz(){
    return sensorDeLuz->leerNiveldeLuz();
}

int Controlador::medirHumedadSuelo(){
    return higrometro->medirHumedad();
}

bool Controlador::empezar(TwoWire *I2C){
    sensorDeLuz->empezar(I2C);
    dht->empezar();
    higrometro->empezar();
    return true;
}