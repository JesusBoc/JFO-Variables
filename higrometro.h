#include "Arduino.h"

#define MEDIDA_AIRE 3631
#define MEDIDA_AGUA 1978

class Higrometro
{
private:
    byte pin;
    int convertir(int valor);
public:
    Higrometro(byte pin);
    void empezar();
    int medirHumedad();
};