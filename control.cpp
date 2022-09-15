#include "control.h"

Controlador::Controlador(int aout, int Dat, int SPI){
    pinMode(aout, INPUT);
    pinMode(Dat, INPUT);
}