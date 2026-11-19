#ifndef GESTOR_CSV_H
#define GESTOR_CSV_H

#include <string>
#include "nodo.h"

class GestorCSV {
private:
    static std::string limpiarEspacios(const std::string& texto);

public:
    static Nodo* cargarListaDesdeCSV(const std::string& rutaArchivo);
    static bool guardarListaEnCSV(const std::string& rutaArchivo, Nodo* cabezaLista);
};

#endif