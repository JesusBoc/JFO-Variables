#ifndef CONTROL_H
#define CONTROL_H
#include "SensorDeLuz.h"
#include "SensorDHT.h"
#include "higrometro.h"
#include "Archivos.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#ifndef SSID
#define SSID "Test"
#define PASSWORD "12345678"
#endif

#define DIRECCION_DE_LOG "/log.txt"

struct medidas{
    float temperatura = NAN;
    float humedadDelAire = NAN;
    float lux = NAN;
    float humedadDelSuelo = NAN;
};

class Controlador
{
private:
    // Objeto del higrómetro
    Higrometro* higrometro;
    // Objeto del sensor de luz
    SensorDeLuz* sensorDeLuz;
    // Objeto del sensor de humedad y temperatura del aire
    SensorDHT* dht;
    // Objeto del gestor de archivos del sistema
    Archivos* archivos;
    // Struct con las ultimas mediciones
    medidas ultimaMedicion;
    // Objeto del servidor
    WebServer* servidor;
public:
    File entregarArchivo(String direccion);
    Controlador(int AOut, int Dat);
    bool empezar(TwoWire *I2C = nullptr);
    float medirHumedad(bool forzar = false);
    float medirTemperatura(bool forzar = false);
    float calcularSensacionTermica();
    float medirLuz();
    int medirHumedadSuelo();
    bool agregarAlLog(String log);
    bool resetearLog();
    medidas obtenerMedidas();
    void onNotFound(WebServer::THandlerFunction fn);
    void enviarArchivo(const char *uri, const char *direccion);
    void responderConArchivo(File archivo,String tipo_contenido);
    void enviarTexto(int codigo, const char *tipo_contenido, String contenido);
    void agregarFuncion(const char *uri, WebServer::THandlerFunction fn);
    void manejarClienteWeb();
    WebServer* obtenerObjServidor();
};
#endif