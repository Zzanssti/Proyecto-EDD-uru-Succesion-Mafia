#include "gestor_csv.h"
#include <fstream>
#include <sstream>

std::string GestorCSV::limpiarEspacios(const std::string& texto) {
    size_t inicio = 0;
    while (inicio < texto.size() && (texto[inicio] == ' ' || texto[inicio] == '\t' || texto[inicio] == '\r' || texto[inicio] == '\n')) {
        inicio++;
    }
    size_t fin = texto.size();
    while (fin > inicio && (texto[fin - 1] == ' ' || texto[fin - 1] == '\t' || texto[fin - 1] == '\r' || texto[fin - 1] == '\n')) {
        fin--;
    }
    return texto.substr(inicio, fin - inicio);
}

Nodo* GestorCSV::cargarListaDesdeCSV(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) return nullptr;

    std::string linea;
    bool esEncabezado = true;
    Nodo* cabeza = nullptr;
    Nodo* cola = nullptr;

    while (std::getline(archivo, linea)) {
        linea = limpiarEspacios(linea);
        if (linea.empty()) continue;

        if (esEncabezado) {
            esEncabezado = false;
            continue;
        }

        std::stringstream ss(linea);
        std::string campo;
        std::string campos[10];
        int idx = 0;

        while (std::getline(ss, campo, ',') && idx < 10) {
            campos[idx++] = limpiarEspacios(campo);
        }

        if (idx < 10) continue;

        Nodo* nuevo = new Nodo();
        nuevo->id = std::stoi(campos[0]);
        nuevo->name = campos[1];
        nuevo->last_name = campos[2];
        nuevo->gender = campos[3].empty() ? 'H' : campos[3][0];
        nuevo->age = std::stoi(campos[4]);
        nuevo->id_boss = std::stoi(campos[5]);
        nuevo->is_dead = std::stoi(campos[6]);
        nuevo->in_jail = std::stoi(campos[7]);
        nuevo->was_boss = std::stoi(campos[8]);
        nuevo->is_boss = std::stoi(campos[9]);

        if (!cabeza) {
            cabeza = cola = nuevo;
        } else {
            cola->sig_lista = nuevo;
            cola = nuevo;
        }
    }

    archivo.close();
    return cabeza;
}

bool GestorCSV::guardarListaEnCSV(const std::string& rutaArchivo, Nodo* cabezaLista) {
    std::ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) return false;

    archivo << "id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss\n";
    Nodo* actual = cabezaLista;
    while (actual) {
        archivo << actual->id << ","
                << actual->name << ","
                << actual->last_name << ","
                << actual->gender << ","
                << actual->age << ","
                << actual->id_boss << ","
                << actual->is_dead << ","
                << actual->in_jail << ","
                << actual->was_boss << ","
                << actual->is_boss << "\n";
        actual = actual->sig_lista;
    }

    archivo.close();
    return true;
}