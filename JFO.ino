#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "Wire.h"
#include "SensorDHT.h"
#include "SensorDeLuz.h"

#define DHT11TESTPIN 26
float temperatura, humedad;
SensorDHT dht(DHT11TESTPIN);
SensorDeLuz sensor;
TwoWire I2C = TwoWire(0);

void setup()
{
    Serial.begin(115200);
    dht.empezar();
    I2C.begin();
    sensor.empezar(&I2C);
}
void loop()
{
    temperatura = dht.leerTemperatura();
    humedad = dht.leerHumedad();
    Serial.print("Temperatura: ");
    Serial.println(temperatura);
    Serial.print("Humedad: ");
    Serial.print(humedad);
    Serial.println(" %");
    float lux = sensor.leerNiveldeLuz();
    Serial.print("Luz: ");
    Serial.print(lux);
    Serial.println(" lx");
    delay(3000);
}