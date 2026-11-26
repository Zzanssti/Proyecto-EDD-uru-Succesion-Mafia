#include <iostream>
#include <string>
#include "arbol_mafia.h"

int main() {
ArbolMafia familia;
std::string rutaCSV = "bin/familia.csv";

// Soporte por si el ejecutable se corre desde dentro de bin/
if (!familia.cargarDesdeCSV(rutaCSV)) {
    rutaCSV = "familia.csv";
    if (!familia.cargarDesdeCSV(rutaCSV)) {
        std::cerr << "[ERROR] No se pudo abrir 'bin/familia.csv' ni 'familia.csv'.\n";
        return 1;
    }
}

int opcion = -1;
do {
    std::cout << "\n================ SISTEMA DE LA FAMILIA ================\n";
    std::cout << "1. Mostrar linea de sucesion actual (solo vivos)\n";
    std::cout << "2. Mostrar todos los miembros (incluidos fallecidos/presos)\n";
    std::cout << "3. Modificar datos de un miembro (excepto ID e ID del jefe)\n";
    std::cout << "4. Simular muerte del jefe actual\n";
    std::cout << "5. Simular encarcelamiento del jefe actual\n";
    std::cout << "6. Simular que el jefe actual supera los 70 años\n";
    std::cout << "7. Guardar cambios actuales en el CSV\n";
    std::cout << "8. Recargar datos originales desde CSV\n";
    std::cout << "0. Salir del programa\n";
    std::cout << "Seleccione una opcion: ";

    if (!(std::cin >> opcion)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Entrada invalida. Ingrese un numero.\n";
        continue;
    }

    switch (opcion) {
        case 1:
            familia.mostrarLineaSucesion();
            break;
        case 2:
            familia.mostrarTodosLosMiembros();
            break;
        case 3: {
            int idBuscar;
            std::cout << "Ingrese el ID del miembro a editar: ";
            std::cin >> idBuscar;
            familia.editarNodo(idBuscar);
            break;
        }
        case 4:
            familia.simularEventoJefe(1);
            break;
        case 5:
            familia.simularEventoJefe(2);
            break;
        case 6:
            familia.simularEventoJefe(3);
            break;
        case 7:
            if (familia.guardarEnCSV(rutaCSV)) {
                std::cout << "[OK] Datos guardados correctamente en " << rutaCSV << "\n";
            } else {
                std::cout << "[ERROR] No se pudo guardar el archivo.\n";
            }
            break;
        case 8:
            if (familia.cargarDesdeCSV(rutaCSV)) {
                std::cout << "[OK] Arbol reconstruido desde " << rutaCSV << "\n";
            }
            break;
        case 0:
            std::cout << "Cerrando sistema de sucesion...\n";
            break;
        default:
            std::cout << "Opcion no reconocida.\n";
    }
} while (opcion != 0);

return 0;
}