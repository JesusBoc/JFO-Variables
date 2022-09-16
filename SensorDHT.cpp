#include "SensorDHT.h"

#define INTERVALO_MINIMO 2000
#define TIMEOUT UINT32_MAX

SensorDHT::SensorDHT(byte pin)
{
    this->pin = pin;
    ciclosMaximos = microsecondsToClockCycles(1000);
}

/**
 * @brief Configura el pin del sensor y define los tiempos de pull-up
 *
 * @param useg
 *        Opcionalmente pasa el tiempo de pull-up, en microsegundos, antes que la medicion del sensor empiece. Por defecto son 55 us
 */
void SensorDHT::empezar(byte useg)
{
    pinMode(pin, INPUT_PULLUP);
    ultimaMedicion = millis() - INTERVALO_MINIMO;
    tiempoDePull = useg;
}

/**
 * @brief Metodo para leer la temperatura usando la forma de lectura del datasheet del sensor
 *
 * @param forzar booleano que define si se va a forzar una nueva medida
 * @return float el valor de la temperatura medida en grados Celsius
 */
float SensorDHT::leerTemperatura(bool forzar)
{
    float f = NAN;

    if (leer(forzar))
    {
        f = data[2];
        if (data[3] & 0x80)
        {
            f = -1 - f;
        }
        f += (data[3] & 0x0f) * 0.1;
    }
    return f;
}

/**
 * @brief Metodo para leer la temperatura usando la forma de lectura del datasheet del sensor
 *
 * @param forzar booleano para definir si se mide aunque no haya pasado el tiempo minimo
 * @return float el valor de la humedad relativa en porcentaje
 */
float SensorDHT::leerHumedad(bool forzar)
{
    float f;
    f = leer(forzar) ? f = data[0] + data[1] * 0.1 : NAN;
    return f;
}

/**
 * @brief Convierte la temperatura de grados celsius a Fahrenheit
 *
 * @param c La temperatura en grados celsius
 * @return float La temperatura en grados fahrenheit
 */
float SensorDHT::convertirCaF(float c)
{
    return c * 1.8 + 32;
}

/**
 * @brief Convierte la temperatura de grados Fahrenheit a celsius
 *
 * @param f La temperatura en grados fahrenheit
 * @return float La temperatura en grados celsius
 */
float SensorDHT::convertirFaC(float f)
{
    return (f - 32) * 0.55555;
}

float SensorDHT::calcularSensacionTermica()
{
    float st = calcularSensacionTermica(leerTemperatura(), leerHumedad());
    return st;
}

float SensorDHT::calcularSensacionTermica(float temperatura, float humedadRelativa)
{
    float st;

    temperatura = convertirCaF(temperatura);
    st = 0.5 * (temperatura + 61.0 + ((temperatura - 68.0) * 1.2) + (humedadRelativa * 0.094));

    if (st > 79)
    {
        st = -42.379 + 2.04901523 * temperatura + 10.14333127 * humedadRelativa +
             -0.22475541 * temperatura * humedadRelativa +
             -0.00683783 * pow(temperatura, 2) +
             -0.05481717 * pow(humedadRelativa, 2) +
             0.00122874 * pow(temperatura, 2) * humedadRelativa +
             0.00085282 * temperatura * pow(humedadRelativa, 2) +
             -0.00000199 * pow(temperatura, 2) * pow(humedadRelativa, 2);

        if ((humedadRelativa < 13) && (temperatura >= 80.0) &&
            (temperatura <= 112.0))
            st -= ((13.0 - humedadRelativa) * 0.25) *
                  sqrt((17.0 - abs(temperatura - 95.0)) * 0.05882);

        else if ((humedadRelativa > 85.0) && (temperatura >= 80.0) &&
                 (temperatura <= 87.0))
            st += ((humedadRelativa - 85.0) * 0.1) * ((87.0 - temperatura) * 0.2);
    }
    return convertirFaC(st);
}

/**
 * @brief Pedimos la medicion del sensor usando la linea de datos del mismo
 * 
 * @param forzar verdadero si queremos forzar una medicion aunque se haya tomado otra antes de 2s
 * @return true Si los datos se leyeron correctamente
 * @return false si hubo algun error al leer los datos
 */
bool SensorDHT::leer(bool forzar)
{
    // Revisamos si el sensor fue medido en un tiempo menor al intervalo minimo
    unsigned long tiempo = millis();
    if (!forzar && ((tiempo - ultimaMedicion) < INTERVALO_MINIMO))
    {
        return ultimoResultado; // Devolvemos el ultimo valor medido correctamente
    }
    ultimaMedicion = tiempo;

    // Reiniciamos los 40 bits de datos recibidos
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    pinMode(pin, INPUT_PULLUP);
    delay(1);

    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(20);

    uint32_t ciclos[80];

    pinMode(pin, INPUT_PULLUP);

    delayMicroseconds(tiempoDePull);

    //Desactivamos interrupciones para evitar malas mediciones
    noInterrupts();

    if (esperarPulso(LOW) == TIMEOUT)
    {
        ultimoResultado = false;
        return ultimoResultado;
    }
    if (esperarPulso(HIGH) == TIMEOUT)
    {
        ultimoResultado = false;
        return ultimoResultado;
    }

    for (int i = 0; i < 80; i += 2)
    {
        ciclos[i] = esperarPulso(LOW);
        ciclos[i + 1] = esperarPulso(HIGH);
    }
    // Las activamos nuevamente
    interrupts();
    for(int i = 0; i < 40; i++){
        unsigned long ciclosEnLow = ciclos[2 * i];
        unsigned long ciclosEnHigh = ciclos[2 * i + 1];
        if((ciclosEnLow == TIMEOUT) || (ciclosEnHigh == TIMEOUT)){
            ultimoResultado = false;
            return ultimoResultado;
        }
        data[i / 8] <<= 1;
        if (ciclosEnHigh > ciclosEnLow) data[i/8] |= 1;
    }
    ultimoResultado = false;
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)){
        ultimoResultado = true;
    }
    return ultimoResultado;
}

/**
 * @brief Se espera un pulso con el nivel logico deseado
 * 
 * @param nivel Alto o bajo; 1 o 0 logicos
 * @return unsigned long el tiempo que demora el pin del sensor en ese nivel
 */
unsigned long SensorDHT::esperarPulso(bool nivel){
    uint32_t count = 0;
    while( digitalRead(pin) == nivel){
        if(count++ >= ciclosMaximos) return TIMEOUT;
    }
    return count;
}