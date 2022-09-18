#include "Archivos.h"

/**
 * @brief Construye un objeto de la clase Archivos
 * 
 */
Archivos::Archivos(){
    manejadorDeArchivos = &SD;
    SD.begin();
}
/**
 * @brief Este metodo permite escribir un archivo en la tarjeta SD montada usando el protocolo SPI
 * 
 * @param direccion La direccion del archivo a escribir
 * @param contenido El contenido del archivo a escribir
 * @return true Si se pudo escribir el archivo
 * @return false Si no se pudo escribir el archivo
 */
bool Archivos::escribirArchivo(const char *direccion, const char *contenido){
    Serial.printf("Escribiendo archivo: %s\n", direccion);

    File archivo = manejadorDeArchivos->open(direccion, FILE_WRITE);
    if(!archivo){
        Serial.println("No se pudo abrir el archivo");
        return false;
    }
    if(archivo.print(contenido)){
        Serial.println("Archivo creado");
        archivo.close();
        return true;
    } 
    Serial.println("No se pudo escribir");
    archivo.close();
    return false;
}
bool Archivos::agregarAlArchivo(const char *direccion, const char *contenido){
    Serial.printf("Escribiendo en el archivo: %s\n", direccion);

    File archivo = manejadorDeArchivos->open(direccion, FILE_APPEND);
    if(!archivo){
        Serial.println("No se pudo abrir el archivo para agregar");
        return false;
    }
    if(archivo.print(contenido)){
        Serial.println("Contenido agregado");
        archivo.close();
        return true;
    } 
    Serial.println("No se pudo agregar al archivo");
    archivo.close();
    return false;
}
bool Archivos::empezar(){
    if(!manejadorDeArchivos->begin()){
        Serial.printf("Fallo al montar la tarjeta SD");
        return false;
    }
    return true;
}
SDFS Archivos::sd(){
    return *manejadorDeArchivos;
}