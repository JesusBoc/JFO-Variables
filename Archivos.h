#include "FS.h"
#include "SD.h"
#include "SPI.h"

#ifndef ARCHIVOS_H
#define ARCHIVOS_H
class Archivos
{
private:
    SDFS* manejadorDeArchivos;
public:
    Archivos();
    bool empezar();
    bool escribirArchivo(const char *direccion, String contenido);
    bool agregarAlArchivo(const char *direccion, String contenido);
    bool eliminarArchivo(const char *direccion);
    SDFS sd();
};

#endif