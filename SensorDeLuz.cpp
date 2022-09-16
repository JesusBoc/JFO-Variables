// Esta es una implementación (adaptada al proyecto) de la librería BH1750 escrita por Christopher Laws.

#include "SensorDeLuz.h"

/**
 * @brief Constuir un nuevo objeto de la clase Sensor De Luz
 * 
 * @param direccion La direccion I2C del sensor
 */
SensorDeLuz::SensorDeLuz(byte direccion)
{
    direccionI2C = direccion;
    I2C = &Wire;
}

/// @brief Con este metodo se pretende empezar las mediciones y permitir recibir informacion del sensor
/// @param modo El modo de medicion (alta/baja resolucion - Contiinua o de una sola medida)
/// @param direccion La direccion I2C del sensor
/// @param i2c El objeto encargado de gestionar la comunicacion I2C con el sensor
/// @return
bool SensorDeLuz::empezar(TwoWire *i2c, Modo modo, byte direccion)
{
    if(i2c) I2C = i2c;
    direccionI2C = direccion;
    return (configurar(modo) && definirTiempoDeMedida());
}


/// @brief Aqui se intenta definir el tiempo de medida interno del sensor, usando el metodo descrito en el datasheet
/// @return Verdadero si se pudo definir, falso si no
bool SensorDeLuz::definirTiempoDeMedida()
{
    byte ack = 5;
    I2C->beginTransmission(direccionI2C);
    I2C->write((0b01000 << 3) | (69 >> 5));
    ack = I2C->endTransmission();
    I2C->beginTransmission(direccionI2C);
    I2C->write((0b011 << 5) | (69 & 0b11111));
    ack = ack | I2C->endTransmission();
    I2C->beginTransmission(direccionI2C);
    I2C->write(MODO);
    ack = ack | I2C->endTransmission();

    delay(10);

    bool revisionack = revisarack(ack);
    if (revisionack)
        tiempodDeMedida = 69;
    return revisionack;
}

/// @brief Se configura el sensor en el modo especificado
/// @param modo El modo de medida
/// @return Verdadero si se pudo configurar, falso si no
bool SensorDeLuz::configurar(Modo modo)
{
    byte ack = 5;

    switch (modo)
    {
    case SensorDeLuz::CONTINUOUS_HIGH_RES_MODE:
    case SensorDeLuz::CONTINUOUS_HIGH_RES_MODE_2:
    case SensorDeLuz::CONTINUOUS_LOW_RES_MODE:
    case SensorDeLuz::ONE_TIME_HIGH_RES_MODE:
    case SensorDeLuz::ONE_TIME_HIGH_RES_MODE_2:
    case SensorDeLuz::ONE_TIME_LOW_RES_MODE:

        I2C->beginTransmission(direccionI2C);
        I2C->write((uint8_t)modo);
        ack = I2C->endTransmission();

        delay(10);
        break;
    default:
        Serial.println(F("[BH1750] ERROR: Modo invalido"));
    }
    bool revisionack = revisarack(ack);
    if (revisionack)
    {
        MODO = modo;
        ultimaMedicion = millis();
    }
    return revisionack;
}

/// @brief Revisa si ha pasado suficiente tiempo para medir un valor nuevo
/// @param esperaMaxima Un valor booleano que define si se espera el tiempo típico o el máximo
/// @return Un booleano que dice si se puede hacer una nueva medicion
bool SensorDeLuz::listoParaMedir(bool esperaMaxima)
{
    unsigned long delaytime = 0;
    switch (MODO)
    {
    case SensorDeLuz::CONTINUOUS_HIGH_RES_MODE:
    case SensorDeLuz::ONE_TIME_HIGH_RES_MODE:
    case SensorDeLuz::ONE_TIME_HIGH_RES_MODE_2:
    case SensorDeLuz::CONTINUOUS_HIGH_RES_MODE_2:
        delaytime = esperaMaxima ? 180 : 120;
        break;
    case SensorDeLuz::ONE_TIME_LOW_RES_MODE:
    case SensorDeLuz::CONTINUOUS_LOW_RES_MODE:
        delaytime = esperaMaxima ? 24 : 16;
        break;
    default:
        break;
    }
    unsigned long tiempoActual = millis();
    if (tiempoActual - ultimaMedicion >= delaytime)
        return true;

    return false;
}

/**
 * @brief Leer el nivel de luz del sensor
 *
 * @return Nivel de luz en luxes (0.0 ~ 54612,5)
 * -1 : Valor invalido
 * -2 : El sensor no esta configurado
 */
float SensorDeLuz::leerNiveldeLuz()
{
    if (MODO == UNCONFIGURED)
    {
        Serial.println(F("[BH1750] El dispositivo no esta configurado"));
        return -2.0;
    }

    float nivel = -1.0;

    if (2 == I2C->requestFrom((int)direccionI2C, (int)2))
    {
        unsigned int tmp = 0;
        tmp = I2C->read();
        tmp <<= 8;
        tmp |= I2C->read();
        nivel = tmp;
    }
    ultimaMedicion = millis();
    if (nivel == -1.0)
        return nivel;
    if (MODO == ONE_TIME_HIGH_RES_MODE_2 || MODO == CONTINUOUS_HIGH_RES_MODE_2)
        nivel /= 2;

    nivel /= factorDeConversion;

    return nivel;
}

bool SensorDeLuz::revisarack(byte ack)
{
    switch (ack)
    {
    case 0:
        return true;
    case 1: // Muy largo para el buffer de transmision
        Serial.println(F("[BH1750] ERROR: Muy largo para el buffer de transmision"));
        break;
    case 2: // recibido NACK en la transmision de la direccion
        Serial.println(F("[BH1750] ERROR: recibido NACK en la transmision de la direccion"));
        break;
    case 3: // recibido NACK en la transmision de los datos
        Serial.println(F("[BH1750] ERROR: recibido NACK en la transmision de los datos"));
        break;
    case 4: // other error
        Serial.println(F("[BH1750] ERROR: otro error"));
        break;
    default:
        Serial.println(F("[BH1750] ERROR: error indefinido"));
        break;
    }

    return false;
}