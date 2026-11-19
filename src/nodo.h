#ifndef NODO_H
#define NODO_H

#include <string>

struct Nodo {
int id;
std::string name;
std::string last_name;
char gender;       // 'H' o 'M'
int age;
int id_boss;
int is_dead;       // 0 o 1
int in_jail;       // 0 o 1
int was_boss;      // 0 o 1
int is_boss;       // 0 o 1

Nodo* izq;         // Primer sucesor
Nodo* der;         // Segundo sucesor
Nodo* padre;       // Puntero al jefe directo para navegar hacia arriba
Nodo* sig_lista;   // Puntero auxiliar para lista enlazada

Nodo()
    : id(0), gender('H'), age(0), id_boss(0),
      is_dead(0), in_jail(0), was_boss(0), is_boss(0),
      izq(nullptr), der(nullptr), padre(nullptr), sig_lista(nullptr) {}
};

#endif