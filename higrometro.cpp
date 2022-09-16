#include "higrometro.h"

Higrometro::Higrometro(byte pin){
    this->pin = pin;
}

void Higrometro::empezar(){
    pinMode(pin,INPUT);
}

int Higrometro::medirHumedad(){
    int medicion = analogRead(pin);
    return convertir(medicion);
}

int Higrometro::convertir(int valor){
    return map(valor, MEDIDA_AIRE, MEDIDA_AGUA, 0, 100);
}