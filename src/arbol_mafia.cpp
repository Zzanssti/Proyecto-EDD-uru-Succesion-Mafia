#include "arbol_mafia.h"
#include "gestor_csv.h"
#include <iostream>

ArbolMafia::ArbolMafia() : raiz(nullptr), cabezaLista(nullptr) {}

ArbolMafia::~ArbolMafia() {
liberarMemoria();
}

void ArbolMafia::liberarMemoria() {
Nodo* actual = cabezaLista;
while (actual) {
Nodo* sig = actual->sig_lista;
delete actual;
actual = sig;
}
cabezaLista = nullptr;
raiz = nullptr;
}

Nodo* ArbolMafia::buscarPorId(int id) const {
Nodo* actual = cabezaLista;
while (actual) {
if (actual->id == id) return actual;
actual = actual->sig_lista;
}
return nullptr;
}

Nodo* ArbolMafia::obtenerJefeActual() const {
Nodo* actual = cabezaLista;
while (actual) {
if (actual->is_boss == 1 && actual->is_dead == 0) return actual;
actual = actual->sig_lista;
}
actual = cabezaLista;
while (actual) {
if (actual->is_boss == 1) return actual;
actual = actual->sig_lista;
}
return nullptr;
}

bool ArbolMafia::cargarDesdeCSV(const std::string& rutaCSV) {
liberarMemoria();
cabezaLista = GestorCSV::cargarListaDesdeCSV(rutaCSV);
if (!cabezaLista) return false;

Nodo* actual = cabezaLista;
while (actual) {
    if (actual->id_boss == 0 || actual->id_boss == -1) {
        raiz = actual;
    } else {
        Nodo* jefe = buscarPorId(actual->id_boss);
        if (jefe) {
            actual->padre = jefe;
            if (!jefe->izq) {
                jefe->izq = actual;
            } else if (!jefe->der) {
                jefe->der = actual;
            }
        }
    }
    actual = actual->sig_lista;
}
return true;
}

bool ArbolMafia::guardarEnCSV(const std::string& rutaCSV) const {
return GestorCSV::guardarListaEnCSV(rutaCSV, cabezaLista);
}

void ArbolMafia::imprimirArbolVivos(Nodo* nodo, const std::string& prefijo, bool esUltimo) const {
if (!nodo) return;

if (nodo->is_dead == 0) {
    std::cout << prefijo << (esUltimo ? "└── " : "├── ")
              << "[ID:" << nodo->id << "] " << nodo->name << " " << nodo->last_name
              << " (" << nodo->gender << ", " << nodo->age << " años)";
    if (nodo->is_boss == 1) std::cout << "  <-- [JEFE ACTUAL]";
    if (nodo->in_jail == 1) std::cout << "  [EN CARCEL]";
    if (nodo->was_boss == 1) std::cout << "  [EX-JEFE]";
    std::cout << "\n";

    std::string nuevoPrefijo = prefijo + (esUltimo ? "    " : "│   ");
    if (nodo->izq && nodo->der) {
        imprimirArbolVivos(nodo->izq, nuevoPrefijo, false);
        imprimirArbolVivos(nodo->der, nuevoPrefijo, true);
    } else if (nodo->izq) {
        imprimirArbolVivos(nodo->izq, nuevoPrefijo, true);
    } else if (nodo->der) {
        imprimirArbolVivos(nodo->der, nuevoPrefijo, true);
    }
} else {
    if (nodo->izq) imprimirArbolVivos(nodo->izq, prefijo, nodo->der == nullptr);
    if (nodo->der) imprimirArbolVivos(nodo->der, prefijo, true);
}
}

void ArbolMafia::imprimirOrdenSucesion(Nodo* nodo, int& posicion) const {
if (!nodo) return;
if (nodo->is_dead == 0 && nodo->is_boss == 0) {
std::cout << "  " << posicion++ << ". " << nodo->name << " " << nodo->last_name
<< " (ID: " << nodo->id << ", Edad: " << nodo->age << ")"
<< (nodo->in_jail == 1 ? " [En carcel - en espera]" : " [Disponible]")
<< "\n";
}
imprimirOrdenSucesion(nodo->izq, posicion);
imprimirOrdenSucesion(nodo->der, posicion);
}

void ArbolMafia::mostrarLineaSucesion() const {
std::cout << "\n======================================================\n";
std::cout << "       LINEA DE SUCESION ACTUAL (SOLO VIVOS)\n";
std::cout << "======================================================\n";

Nodo* jefe = obtenerJefeActual();
if (jefe && jefe->is_dead == 0) {
    std::cout << "DON / JEFE ACTUAL: " << jefe->name << " " << jefe->last_name
              << " (ID: " << jefe->id << ", Edad: " << jefe->age << ")\n\n";
} else {
    std::cout << "DON / JEFE ACTUAL: Ninguno activo\n\n";
}

std::cout << "Estructura Jerarquica Viva:\n";
imprimirArbolVivos(raiz, "", true);

if (jefe && jefe->is_dead == 0) {
    std::cout << "\nOrden Directo de Sucesion en la Rama del Jefe:\n";
    int pos = 1;
    imprimirOrdenSucesion(jefe->izq, pos);
    imprimirOrdenSucesion(jefe->der, pos);
    if (pos == 1) {std::cout << "  (El jefe actual no tiene descendientes vivos en su rama; aplicara herencia lateral)\n";
    }
}
std::cout << "======================================================\n";
}

void ArbolMafia::mostrarTodosLosMiembros() const {
std::cout << "\n--- REGISTRO COMPLETO DE LA FAMILIA ---\n";
Nodo* actual = cabezaLista;
while (actual) {
std::cout << "ID: " << actual->id
<< " | " << actual->name << " " << actual->last_name
<< " | Genero: " << actual->gender
<< " | Edad: " << actual->age
<< " | Jefe(ID): " << actual->id_boss
<< " | Estado: " << (actual->is_dead ? "MUERTO" : "VIVO")
<< " | Carcel: " << (actual->in_jail ? "SI" : "NO")
<< " | Ex-Jefe: " << (actual->was_boss ? "SI" : "NO")
<< " | Jefe Actual: " << (actual->is_boss ? "SI" : "NO")
<< "\n";
actual = actual->sig_lista;
}
}