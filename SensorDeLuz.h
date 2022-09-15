// Esta es una implementación (adaptada al proyecto) de la librería BH1750 escrita por Christopher Laws.

#include <Wire.h>

// Estado inactivo
#define BH1750_POWER_DOWN 0x00

// Estado activo(Esperando medida)
#define BH1750_POWER_ON 0x01

// Reiniciar el valor de registro de datos - no se acepta en el estado inactivo
#define BH1750_RESET 0x07

// Se define la direccion 0x23 como la predeterminada
#define DEFAULT_ADDRESS 0x23

class SensorDeLuz
{
public:
    enum Modo
    {
        // El mismo valor que el estado inactivo
        UNCONFIGURED = 0,
        // Medida con resolucion de 1 lux. Tiempo de medida aproximado de 120mx.
        CONTINUOUS_HIGH_RES_MODE = 0x10,
        // Medida con resolucion de 0.5 lux. Tiempo de medida aproximado de 120mx.
        CONTINUOUS_HIGH_RES_MODE_2 = 0x11,
        // Medida con resolucion de 4 lux. Tiempo de medida aproximado de 16mx.
        CONTINUOUS_LOW_RES_MODE = 0x13,
        // Medida con resolucion de 1 lux. Tiempo de medida aproximado de 120mx.
        ONE_TIME_HIGH_RES_MODE = 0x20,
        // Medida con resolucion de 0.5 lux. Tiempo de medida aproximado de 120mx.
        ONE_TIME_HIGH_RES_MODE_2 = 0x21,
        // Medida con resolucion de 4 lux. Tiempo de medida aproximado de 16mx.
        ONE_TIME_LOW_RES_MODE = 0x23
    };
    SensorDeLuz(byte direccion = DEFAULT_ADDRESS);
    bool empezar(Modo modo = CONTINUOUS_HIGH_RES_MODE, byte direccion = DEFAULT_ADDRESS, TwoWire *i2c = nullptr);
    bool configurar(Modo modo);
    bool listoParaMedir(bool esperaMaxima = false);
    int leerNiveldeLuz();

private:
    byte direccionI2C;
    byte tiempodDeMedida;
    // Este es el factor usado para calcular la cantidad de lux. El valor tipico es de 1.2 pero puede variar entre 0.96 y 1.44
    const float factorDeConversion = 1.2;
    Modo MODO = UNCONFIGURED;
    TwoWire* I2C;
    unsigned long ultimaMedicion;
};