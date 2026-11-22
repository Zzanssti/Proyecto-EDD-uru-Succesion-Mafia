#ifndef ARBOL_MAFIA_H
#define ARBOL_MAFIA_H

#include <string>
#include "nodo.h"

class ArbolMafia {
private:
Nodo* raiz;
Nodo* cabezaLista;

void liberarMemoria();
Nodo* buscarPorId(int id) const;
Nodo* obtenerJefeActual() const;

void imprimirArbolVivos(Nodo* nodo, const std::string& prefijo, bool esUltimo) const;
void imprimirOrdenSucesion(Nodo* nodo, int& posicion) const;
public:
ArbolMafia();
~ArbolMafia();

bool cargarDesdeCSV(const std::string& rutaCSV);
bool guardarEnCSV(const std::string& rutaCSV) const;
void mostrarLineaSucesion() const;
void mostrarTodosLosMiembros() const;
};

#endif