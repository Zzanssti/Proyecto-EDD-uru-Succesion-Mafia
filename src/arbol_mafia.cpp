#include "arbol_mafia.h"
#include "gestor_csv.h"
#include "cola.h"
#include <climits>
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
verificarSucesion();
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
bool ArbolMafia::esElegible(Nodo* n, bool permitirCarcel) const {
    if (!n || n->is_dead == 1) return false;
    if (!permitirCarcel && n->in_jail == 1) return false;
    if (!permitirCarcel && n->age > 70) return false;
    return true;
}

Nodo* ArbolMafia::buscarPrimeroEnSubarbol(Nodo* subraiz, bool permitirCarcel, bool incluirRaiz) const {
    if (!subraiz) return nullptr;
    if (incluirRaiz && esElegible(subraiz, permitirCarcel)) {
        return subraiz;
    }
    Nodo* izqRes = buscarPrimeroEnSubarbol(subraiz->izq, permitirCarcel, true);
    if (izqRes) return izqRes;
    return buscarPrimeroEnSubarbol(subraiz->der, permitirCarcel, true);
}

Nodo* ArbolMafia::buscarEnRamaCompanero(Nodo* companero, bool permitirCarcel) const {
    if (!companero) return nullptr;

    Nodo* enHijos = buscarPrimeroEnSubarbol(companero->izq, permitirCarcel, true);
    if (enHijos) return enHijos;

    enHijos = buscarPrimeroEnSubarbol(companero->der, permitirCarcel, true);
    if (enHijos) return enHijos;

    if (esElegible(companero, permitirCarcel)) {
        return companero;
    }
    return nullptr;
}

Nodo* ArbolMafia::seleccionarSucesor(Nodo* jefeActual, bool permitirCarcel) const {
    if (!jefeActual) return nullptr;

    // 3.1: Primer sucesor vivo y libre en su propio árbol
    Nodo* candidato = buscarPrimeroEnSubarbol(jefeActual, permitirCarcel, false);
    if (candidato) return candidato;

    // 3.2 y 3.3: Árbol del compañero sucesor de su jefe, o el compañero mismo si está libre y sin sucesores
    if (jefeActual->padre) {
        Nodo* padre = jefeActual->padre;
        Nodo* companero = (padre->izq == jefeActual) ? padre->der : padre->izq;
        candidato = buscarEnRamaCompanero(companero, permitirCarcel);
        if (candidato) return candidato;

        // 3.4: Árbol del compañero sucesor de su anterior jefe (abuelo)
        if (padre->padre) {
            Nodo* abuelo = padre->padre;
            Nodo* companeroAnterior = (abuelo->izq == padre) ? abuelo->der : abuelo->izq;
            candidato = buscarEnRamaCompanero(companeroAnterior, permitirCarcel);
            if (candidato) return candidato;
        }

        // 3.5: Jefe mas cercano con dos sucesores fuera de la carcel
        candidato = buscarJefeCercanoDosSucesores(jefeActual, permitirCarcel);
    if (candidato) return candidato;

    }
    return nullptr;
}

void ArbolMafia::promoverNuevoJefe(Nodo* jefeAnterior, Nodo* nuevoJefe) {
    if (!nuevoJefe || jefeAnterior == nuevoJefe) return;

    if (jefeAnterior) {
        jefeAnterior->is_boss = 0;
        jefeAnterior->was_boss = 1;
    }
    nuevoJefe->is_boss = 1;

    std::cout << "\n[SUCESION AUTOMATICA] El nuevo jefe de la familia es: "
              << nuevoJefe->name << " " << nuevoJefe->last_name
              << " (ID: " << nuevoJefe->id << ")"
              << (nuevoJefe->in_jail == 1 ? " [DIRIGIENDO DESDE PRISION]" : "")
              << "\n";
}

void ArbolMafia::verificarSucesion() {
   
Nodo* jefeActual = obtenerJefeActual();
if (!jefeActual) return;

// Regla 3.7: Si el jefe paso de 70 anos o fue a prision
if (jefeActual->is_dead == 0 && (jefeActual->age > 70 || jefeActual->in_jail == 1)) {
    Nodo* sucesorArbol = buscarPrimeroEnSubarbol(jefeActual, false, false);
    if (!sucesorArbol) {
        sucesorArbol = seleccionarSucesor(jefeActual, false);
    }
    if (sucesorArbol) {
        promoverNuevoJefe(jefeActual, sucesorArbol);
    } else if (jefeActual->age > 70 && jefeActual->in_jail == 0) {
        Nodo* encarcelado = seleccionarSucesor(jefeActual, true);
        if (!encarcelado) encarcelado = buscarMasCercanoEncarcelado(jefeActual);
        if (encarcelado) promoverNuevoJefe(jefeActual, encarcelado);
    }
    return;
}

// Reglas 3.1 a 3.6: Si el jefe actual murio
if (jefeActual->is_dead == 1) {
    Nodo* sucesor = seleccionarSucesor(jefeActual, false);

    if (!sucesor) {
        sucesor = seleccionarSucesor(jefeActual, true);
        if (!sucesor) {
            sucesor = buscarMasCercanoEncarcelado(jefeActual);
        }
    }

    if (sucesor) {
        promoverNuevoJefe(jefeActual, sucesor);
    } else {
        jefeActual->is_boss = 0;
        jefeActual->was_boss = 1;
        std::cout << "\n[ALERTA] No quedan miembros vivos disponibles para asumir el liderazgo.\n";
    }


}

}

int ArbolMafia::obtenerProfundidad(Nodo* n) const {
int prof = 0;
while (n && n->padre) {
prof++;
n = n->padre;
}
return prof;
}

int ArbolMafia::distanciaEntreNodos(Nodo* a, Nodo* b) const {
if (!a || !b) return INT_MAX;
int profA = obtenerProfundidad(a);
int profB = obtenerProfundidad(b);
int dist = 0;

while (profA > profB) {
    a = a->padre;
    profA--;
    dist++;
}
while (profB > profA) {
    b = b->padre;
    profB--;
    dist++;
}
while (a && b && a != b) {
    a = a->padre;
    b = b->padre;
    dist += 2;
}
return dist;
}

// Regla 3.5: Jefe mas cercano con dos sucesores fuera de la carcel
Nodo* ArbolMafia::buscarJefeCercanoDosSucesores(Nodo* jefeActual, bool permitirCarcel) const {
if (!raiz || !jefeActual) return nullptr;

ColaNodos cola;
cola.encolar(raiz);

Nodo* mejorCandidato = nullptr;
int menorDistancia = INT_MAX;

while (!cola.estaVacia()) {
    Nodo* actual = cola.desencolar();
    if (actual->izq) cola.encolar(actual->izq);
    if (actual->der) cola.encolar(actual->der);

    if (actual == jefeActual) continue;

    if (actual->izq && actual->der) {
        if (esElegible(actual->izq, permitirCarcel) && esElegible(actual->der, permitirCarcel)) {
            int d = distanciaEntreNodos(jefeActual, actual);
            if (d < menorDistancia) {
                menorDistancia = d;
                mejorCandidato = actual->izq;
            }
        }
    }
}
return mejorCandidato;
}

// Regla 3.6: Respaldo con miembros vivos en la carcel
Nodo* ArbolMafia::buscarMasCercanoEncarcelado(Nodo* jefeActual) const {
if (!raiz || !jefeActual) return nullptr;

ColaNodos cola;
cola.encolar(raiz);

Nodo* mejor = nullptr;
int menorDist = INT_MAX;

while (!cola.estaVacia()) {
    Nodo* actual = cola.desencolar();
    if (actual->izq) cola.encolar(actual->izq);
    if (actual->der) cola.encolar(actual->der);

    if (actual != jefeActual && actual->is_dead == 0 && actual->in_jail == 1) {
        int d = distanciaEntreNodos(jefeActual, actual);
        if (d < menorDist) {
            menorDist = d;
            mejor = actual;
        }
    }
}
return mejor;
}
void ArbolMafia::editarNodo(int id) {
    Nodo* nodo = buscarPorId(id);
    if (!nodo) {
        std::cout << "[ERROR] No existe ningun miembro con ID " << id << ".\n";
        return;
    }

    int opcion = -1;
    do {
        std::cout << "\nEditando a: " << nodo->name << " " << nodo->last_name
                  << " (ID: " << nodo->id << " [Protegido], ID Jefe: " << nodo->id_boss << " [Protegido])\n";
        std::cout << "1. Nombre (" << nodo->name << ")\n";
        std::cout << "2. Apellido (" << nodo->last_name << ")\n";
        std::cout << "3. Genero (" << nodo->gender << ")\n";
        std::cout << "4. Edad (" << nodo->age << ")\n";
        std::cout << "5. Esta muerto / is_dead (" << nodo->is_dead << ")\n";
        std::cout << "6. En prision / in_jail (" << nodo->in_jail << ")\n";
        std::cout << "7. Fue jefe / was_boss (" << nodo->was_boss << ")\n";
        std::cout << "8. Es jefe / is_boss (" << nodo->is_boss << ")\n";
        std::cout << "0. Finalizar edicion y verificar sucesion\n";
        std::cout << "Seleccione campo a modificar: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                std::cout << "Nuevo nombre: ";
                std::cin >> nodo->name;
                break;
            case 2:
                std::cout << "Nuevo apellido: ";
                std::cin >> nodo->last_name;
                break;
            case 3: {
                char g;
                std::cout << "Nuevo genero (H/M): ";
                std::cin >> g;
                if (g == 'H' || g == 'M' || g == 'h' || g == 'm') {
                    nodo->gender = (g == 'h') ? 'H' : (g == 'm' ? 'M' : g);
                } else {
                    std::cout << "Genero invalido. Use H o M.\n";
                }
                break;
            }
            case 4:
                std::cout << "Nueva edad: ";
                std::cin >> nodo->age;
                break;
            case 5:
                std::cout << "is_dead (0 = Vivo, 1 = Muerto): ";
                std::cin >> nodo->is_dead;
                nodo->is_dead = (nodo->is_dead != 0) ? 1 : 0;
                break;
            case 6:
                std::cout << "in_jail (0 = Libre, 1 = Preso): ";
                std::cin >> nodo->in_jail;
                nodo->in_jail = (nodo->in_jail != 0) ? 1 : 0;
                break;
            case 7:
                std::cout << "was_boss (0 = No, 1 = Si): ";
                std::cin >> nodo->was_boss;
                nodo->was_boss = (nodo->was_boss != 0) ? 1 : 0;
                break;
            case 8:
                std::cout << "is_boss (0 = No, 1 = Si): ";
                std::cin >> nodo->is_boss;
                nodo->is_boss = (nodo->is_boss != 0) ? 1 : 0;
                break;
            case 0:
                break;
            default:
                std::cout << "Opcion no valida.\n";
        }
    } while (opcion != 0);

    verificarSucesion();
}