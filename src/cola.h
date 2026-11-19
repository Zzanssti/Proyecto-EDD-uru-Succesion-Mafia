#ifndef COLA_H
#define COLA_H

#include "nodo.h"

struct NodoCola {
Nodo* dato;
NodoCola* siguiente;
NodoCola(Nodo* d) : dato(d), siguiente(nullptr) {}
};

class ColaNodos {
private:
NodoCola* frente;
NodoCola* fondo;

public:
ColaNodos() : frente(nullptr), fondo(nullptr) {}

~ColaNodos() {
    while (!estaVacia()) {
        desencolar();
    }
}

bool estaVacia() const {
    return frente == nullptr;
}

void encolar(Nodo* n) {
    if (!n) return;
    NodoCola* nuevo = new NodoCola(n);
    if (estaVacia()) {
        frente = fondo = nuevo;
    } else {
        fondo->siguiente = nuevo;
        fondo = nuevo;
    }
}

Nodo* desencolar() {
    if (estaVacia()) return nullptr;
    NodoCola* temp = frente;
    Nodo* dato = temp->dato;
    frente = frente->siguiente;
    if (!frente) fondo = nullptr;
    delete temp;
    return dato;
}
};

#endif