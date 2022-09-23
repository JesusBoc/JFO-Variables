#include "control.h"

Controlador::Controlador(int aout, int Dat){
    sensorDeLuz = new SensorDeLuz();
    dht = new SensorDHT(Dat);
    higrometro = new Higrometro(aout);
    archivos = new Archivos();
    servidor = new WebServer(80);
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
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID,PASSWORD);
    Serial.println(WiFi.softAPIP());

    if (MDNS.begin("esp32"))
        Serial.println("Iniciado el MDNS responder");
    servidor->begin();
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
void Controlador::onNotFound(WebServer::THandlerFunction fn){
    servidor->onNotFound(fn);
}
void Controlador::enviarArchivo(const char *uri, const char *direccion){
    fs::SDFS archivosSD = archivos->sd();
    servidor->serveStatic(uri,archivosSD,direccion);
}
void Controlador::enviarTexto(int codigo, const char *tipo_contenido, String contenido){
    servidor->send(codigo,tipo_contenido,contenido);
}
void Controlador::agregarFuncion(const char *uri, WebServer::THandlerFunction fn){
    servidor->on(uri,fn);
}
WebServer* Controlador::obtenerObjServidor(){
    return servidor;
}
void Controlador::manejarClienteWeb(){
    servidor->handleClient();
}
File Controlador::entregarArchivo(String direccion){
    File archivo = archivos->sd().open(direccion);
    return archivo;
}
void Controlador::responderConArchivo(File archivo, String tipo_contenido){
    if(archivo){
        servidor->streamFile(archivo,tipo_contenido);
        archivo.close();
    }
    Serial.println("No pudo enviarse");
}