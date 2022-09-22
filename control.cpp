#include "control.h"

Controlador::Controlador(int aout, int Dat){
    sensorDeLuz = new SensorDeLuz();
    dht = new SensorDHT(Dat);
    higrometro = new Higrometro(aout);
    archivos = new Archivos();
}

float Controlador::medirHumedad(bool forzar){
    float resultado = dht->leerHumedad(forzar);
    ultimaMedicion.humedadDelAire = resultado;
    return resultado;
}

float Controlador::medirTemperatura(bool forzar){
    float resultado = dht->leerTemperatura(forzar);
    ultimaMedicion.temperatura = resultado;
    return resultado;
}

float Controlador::calcularSensacionTermica(){
    return dht->calcularSensacionTermica();
}

float Controlador::medirLuz(){
    float resultado = sensorDeLuz->leerNiveldeLuz();
    ultimaMedicion.lux = resultado;
    return resultado;
}

int Controlador::medirHumedadSuelo(){
    float resultado = higrometro->medirHumedad();
    ultimaMedicion.humedadDelSuelo = resultado;
    return resultado;
}

bool Controlador::empezar(TwoWire *I2C){
    sensorDeLuz->empezar(I2C);
    dht->empezar();
    higrometro->empezar();
    if(sensorDeLuz->empezar(I2C) && archivos->empezar()) return true;
    Serial.println("Algo fallo");
    return false;
}
bool Controlador::agregarAlLog(String log){
    return archivos->agregarAlArchivo(DIRECCION_DE_LOG,log);
}
medidas Controlador::obtenerMedidas(){
    return ultimaMedicion;
}
bool Controlador::resetearLog(){
    return archivos->eliminarArchivo(DIRECCION_DE_LOG);
}