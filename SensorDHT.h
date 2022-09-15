// Esta es una implementación (adaptada al proyecto) de la librería DHT escrita por Adafruit Industries.

#include <Arduino.h>

class SensorDHT
{
public:
    SensorDHT(byte pin);
    void empezar(byte useg = 55);
    float leerTemperatura(bool forzar = false);
    float calcularSensacionTermica();
    float leerHumedad(bool forzar = false);
    bool leer(bool forzar = false);

private:
    byte data[5];
    byte pin;
    unsigned long ultimaMedicion, ciclosMaximos;
    bool ultimoResultado;
    byte tiempoDePull;
    unsigned long esperarPulso(bool nivel);
    float convertirFaC(float f);
    float convertirCaF(float C);
    float calcularSensacionTermica(float temperatura, float humedadRelativa);
};