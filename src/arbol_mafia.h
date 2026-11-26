#ifndef ARBOL_MAFIA_H
#define ARBOL_MAFIA_H

#include <string>
#include "nodo.h"

class ArbolMafia {
private:
int obtenerProfundidad(Nodo* n) const;
int distanciaEntreNodos(Nodo* a, Nodo* b) const;
Nodo* buscarJefeCercanoDosSucesores(Nodo* jefeActual, bool permitirCarcel) const;
Nodo* buscarMasCercanoEncarcelado(Nodo* jefeActual) const;
Nodo* raiz;
Nodo* cabezaLista;
bool esElegible(Nodo* n, bool permitirCarcel) const;
Nodo* buscarPrimeroEnSubarbol(Nodo* subraiz, bool permitirCarcel, bool incluirRaiz) const;
Nodo* buscarEnRamaCompanero(Nodo* companero, bool permitirCarcel) const;
Nodo* seleccionarSucesor(Nodo* jefeActual, bool permitirCarcel) const;
void promoverNuevoJefe(Nodo* jefeAnterior, Nodo* nuevoJefe);
void liberarMemoria();
Nodo* buscarPorId(int id) const;
Nodo* obtenerJefeActual() const;
Nodo* buscarPrimerLibreGlobal() const;
void imprimirArbolVivos(Nodo* nodo, const std::string& prefijo, bool esUltimo) const;
void imprimirOrdenSucesion(Nodo* nodo, int& posicion) const;
public:
ArbolMafia();
~ArbolMafia();
void verificarSucesion();
bool cargarDesdeCSV(const std::string& rutaCSV);
bool guardarEnCSV(const std::string& rutaCSV) const;
void mostrarLineaSucesion() const;
void mostrarTodosLosMiembros() const;
void editarNodo(int id);
void simularEventoJefe(int tipoEvento); // 1:muerte, 2:carcel, 3: edad >70
void verificarSucesion(bool huboLiberacion = false);
};

#endif