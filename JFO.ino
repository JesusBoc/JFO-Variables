#include "control.h"

#define PIN_DHT 26
#define PIN_HIGROMETRO 27
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

void tareaManejarServidor(void *pvParameters);
void tareaManejarMedidas(void *pvParameters);

#pragma region
void configurar();
void mostrarTemperatura();
void mostrarHumedadDelAire();
void mostrarLuz();
void mostrarHumedadDelSuelo();
void separar();
void guardarMedidasEnArchivo();
void esperar(int tiempo = 3000);
void onNotFound();
void mostrarLog();
#pragma endregion

Controlador JFO(PIN_HIGROMETRO, PIN_DHT);
String guardar;
WebServer* manejador = JFO.obtenerObjServidor();
unsigned long tiempo = 0;

void setup()
{
    configurar();
}
void loop()
{
}

#pragma region
void configurar()
{
    Serial.begin(115200);
    Wire.begin();
    JFO.empezar();
    JFO.resetearLog();
    JFO.onNotFound(onNotFound);
    xTaskCreatePinnedToCore(
        tareaManejarMedidas,
        "Medidas",
        4096,
        NULL,
        2,
        NULL,
        ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(
        tareaManejarServidor,
        "Manejador de servidor",
        8192,
        NULL,
        1,
        NULL,
        0);
    JFO.agregarFuncion("/",mostrarLog);
}
void mostrarTemperatura()
{
    float temperatura = JFO.medirTemperatura();
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(F(" °C"));
}
void mostrarHumedadDelAire()
{
    float humedadAire = JFO.medirHumedad();
    Serial.print("Humedad del aire: ");
    Serial.print(humedadAire);
    Serial.println(" %");
}
void mostrarLuz()
{
    float luz = JFO.medirLuz();
    Serial.print("Luz: ");
    Serial.print(luz);
    Serial.println(" lux");
}
void mostrarHumedadDelSuelo()
{
    float humedadSuelo = JFO.medirHumedadSuelo();
    Serial.print("Humedad del suelo: ");
    Serial.print(humedadSuelo);
    Serial.println(" %");
}
void separar()
{
    Serial.println("-----------------------------------------");
}
void guardarMedidasEnArchivo()
{
    medidas datos = JFO.obtenerMedidas();
    String guardar;
    guardar = millis();
    guardar += " " + (String)datos.temperatura;
    guardar += " " + (String)datos.humedadDelAire;
    guardar += " " + (String)datos.lux;
    guardar += " " + (String)datos.humedadDelSuelo + "\n";
    JFO.agregarAlLog(guardar);
}
void esperar(int tiempo)
{
    TickType_t t = tiempo / portTICK_PERIOD_MS;
    vTaskDelay(t);
}
void onNotFound(){
    String message = "Archivo no encontrado\n\n";
    message += "URI: ";
    message += manejador->uri();
    message += "\nMetodo: ";
    message += (manejador->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArgumentos: ";
    message += manejador->args();
    message += "\n";
    for (uint8_t i = 0; i < manejador->args(); i++)
    {
        message += " " + manejador->argName(i) + ": " + manejador->arg(i) + "\n";
    }
    manejador->send(404, "text/plain", message);
}
void mostrarLog(){
    File archivo = JFO.entregarArchivo(DIRECCION_DE_LOG);
    JFO.responderConArchivo(archivo,"text/plain");
}
#pragma endregion

/**
 * @brief Tarea para manejar las medidas de forma asincronica
 *
 * @param pvParameters NULL
 */
void tareaManejarMedidas(void *pvParameters)
{
    (void) pvParameters;
    for (;;)
    {
        esperar();
        separar();
        mostrarTemperatura();
        mostrarHumedadDelAire();
        mostrarHumedadDelSuelo();
        mostrarLuz();
        guardarMedidasEnArchivo();
    }
}
void tareaManejarServidor(void *pvParameters){
    (void) pvParameters;
    for (;;)
    {
        JFO.manejarClienteWeb();
        esperar(2);
    }
}