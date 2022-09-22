#include "control.h"

#define PIN_DHT 26
#define PIN_HIGROMETRO 27

#pragma region 
void configurar();
void mostrarTemperatura();
void mostrarHumedadDelAire();
void mostrarLuz();
void mostrarHumedadDelSuelo();
void separar();
void guardarMedidasEnArchivo();
void esperar(int tiempo = 3000);
#pragma endregion

Controlador JFO(PIN_HIGROMETRO,PIN_DHT);
String guardar;

void setup()
{
    configurar();
}
void loop()
{
    esperar();
    separar();
    mostrarLuz();
    mostrarHumedadDelAire();
    mostrarHumedadDelSuelo();
    mostrarTemperatura();
    guardarMedidasEnArchivo();
}

void configurar(){
    Serial.begin(115200);
    Wire.begin();
    JFO.empezar();
    JFO.resetearLog();
}
void mostrarTemperatura(){
    float temperatura = JFO.medirTemperatura();
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(F(" °C"));
}
void mostrarHumedadDelAire(){
    float humedadAire = JFO.medirHumedad();
    Serial.print("Humedad del aire: ");
    Serial.print(humedadAire);
    Serial.println(" %");
}
void mostrarLuz(){
    float luz = JFO.medirLuz();
    Serial.print("Luz: ");
    Serial.print(luz);
    Serial.println(" lux");
}
void mostrarHumedadDelSuelo(){
    float humedadSuelo = JFO.medirHumedadSuelo();
    Serial.print("Humedad del suelo: ");
    Serial.print(humedadSuelo);
    Serial.println(" %");
}
void separar(){
    Serial.println("-----------------------------------------");
}
void guardarMedidasEnArchivo(){
    medidas datos = JFO.obtenerMedidas();
    String guardar;
    guardar = millis();
    guardar += " " + (String)datos.temperatura;
    guardar += " " + (String)datos.humedadDelAire;
    guardar += " " + (String)datos.lux;
    guardar += " " + (String)datos.humedadDelSuelo + "\n";
    JFO.agregarAlLog(guardar);
}
void esperar(int tiempo){
    delay(tiempo);
}