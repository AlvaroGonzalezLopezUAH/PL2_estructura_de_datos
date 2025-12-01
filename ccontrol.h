#ifndef CCONTROL_H
#define CCONTROL_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iomanip>


struct Pedido {
    int id_libreria;          // 3 cifras
    std::string id_pedido;    // 'P' + 5 cifras
    std::string cod_libro;    // 3 dígitos + letra + 2 dígitos (ej: 963K76)
    std::string materia;
    int unidades;
    std::string fecha_envio;  // DD-MM-2025

};

struct NodoPedido {
    Pedido dato;
    NodoPedido* sig;
    NodoPedido(const Pedido& p) : dato(p), sig(nullptr) {}
};

struct Libreria {
    int id_libreria;       // 3 cifras
    std::string localidad;
    ListaPedidos pedidos;  // lista de pedidos (inicialmente vacía)
};

struct NodoABB {
    Libreria info;
    NodoABB* izq;
    NodoABB* der;
    NodoABB(const Libreria& l) : info(l), izq(nullptr), der(nullptr) {}
};
