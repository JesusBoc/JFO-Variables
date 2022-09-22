#include "control.h"

#define DHT11TESTPIN 26
#define HIGROMETROTESTPIN 27
float temperatura, humedad, humedadSuelo, lux;
Controlador JFO(HIGROMETROTESTPIN,DHT11TESTPIN);
String guardar;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    JFO.empezar();
}
void loop()
{
    guardar = millis();
    Serial.println("-----------------------------------------");
    temperatura = JFO.medirTemperatura();
    humedad = JFO.medirHumedad();
    humedadSuelo = JFO.medirHumedadSuelo();
    lux = JFO.medirLuz();
    Serial.print("Temperatura: ");
    Serial.println(temperatura);
    guardar += " " + (String)temperatura;
    Serial.print("Humedad del aire: ");
    Serial.print(humedad);
    guardar += " " + (String)humedad;
    Serial.println(" %");
    Serial.print("Luz: ");
    Serial.print(lux);
    guardar += " " + (String)lux;
    Serial.println(" lx");
    Serial.print("Humedad del suelo: ");
    Serial.print(humedadSuelo);
    Serial.println(" %");
    guardar += " " + (String)humedadSuelo + "\n";
    Serial.println("-----------------------------------------");
    JFO.agregarAlLog(guardar);
    delay(3000);
}