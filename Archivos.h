#include "FS.h"
#include "SD.h"
#include "SPI.h"

class Archivos
{
private:
    SDFS* manejadorDeArchivos;
public:
    Archivos();
    bool empezar();
    bool escribirArchivo(const char *direccion, const char *contenido);
    bool agregarAlArchivo(const char *direccion, const char *contenido);
    SDFS sd();
};
