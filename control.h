#include "SensorDeLuz.h"

class Controlador{
    private:
        //Pin del higrómetro
        int pinAOut;
        //Pin del DHT11
        int pinDat;
        //Objeto del sensor de luz
        SensorDeLuz sensor;

    public:
        void establecerBaudRate(int baudRate);
};