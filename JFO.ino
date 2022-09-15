#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "Wire.h"
#include "SensorDHT.h"

#define DHT11TESTPIN 26
float temperatura, humedad;
SensorDHT dht(DHT11TESTPIN);

void setup(){
    Serial.begin(115200);
    dht.empezar();
}
void loop(){
    temperatura = dht.leerTemperatura();
    humedad = dht.leerHumedad();
    Serial.print("Temperatura: ");
    Serial.println(temperatura);
    Serial.print("Humedad: ");
    Serial.print(humedad);
    Serial.println(" %");
    delay(3000);
}