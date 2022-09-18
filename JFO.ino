#include "control.h"

#define DHT11TESTPIN 26
#define HIGROMETROTESTPIN 27
float temperatura, humedad, humedadSuelo, lux;
Controlador JFO(HIGROMETROTESTPIN,DHT11TESTPIN);

void setup()
{
    Serial.begin(115200);
    Serial.println("Configurado");
    Wire.begin();
    Serial.println("Configurado");
    JFO.empezar();
    Serial.println("Configurado");
}
void loop()
{
    Serial.println("-----------------------------------------");
    temperatura = JFO.medirTemperatura();
    humedad = JFO.medirHumedad();
    humedadSuelo = JFO.medirHumedadSuelo();
    lux = JFO.medirLuz();
    Serial.print("Temperatura: ");
    Serial.println(temperatura);
    Serial.print("Humedad del aire: ");
    Serial.print(humedad);
    Serial.println(" %");
    Serial.print("Luz: ");
    Serial.print(lux);
    Serial.println(" lx");
    Serial.print("Humedad del suelo: ");
    Serial.print(humedadSuelo);
    Serial.println(" %");
    Serial.println("-----------------------------------------");
    delay(3000);
}