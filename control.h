#include "SensorDeLuz.h"

class Controlador{
    private:
        //Pin del higrómetro
        int pinAOut;
        //Pin del DHT11
        int pinDat;
        //Objeto del sensor de luz
        SensorDeLuz sensor;
        //Pin del SPI para la tarjeta SD
        int pinSPI;
    public:
        void establecerBaudRate(int baudRate);
        Controlador(int AOut, int Dat, int pinSPI);
};