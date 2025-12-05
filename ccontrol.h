#ifndef CCONTROL_H
#define CCONTROL_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iomanip>

using namespace std;

const string MATERIAS[] = {"Matemáticas","Física","Tecnología","Música","Historia","Lengua"};
const int N_MATERIAS = 6;

const string LOCALIDADES[] = {
    "Móstoles","Alcalá","Leganés","Fuenlabrada","Getafe","Alcorcón","Torrejón","Parla",
    "Alcobendas","Coslada","Pozuelo","Rivas","Valdemoro","Majadahonda","Aranjuez","Arganda",
    "Boadilla","Pinto","Colmenar","Tres Cantos"
};
const int N_LOCALIDADES = 20;

struct Pedido {
    int id_libreria;          // 3 cifras
    string id_pedido;    // 'P' + 5 cifras
    string cod_libro;    // 3 dígitos + letra + 2 dígitos (ej: 963K76)
    string materia;
    int unidades;
    string fecha_envio;  // DD-MM-2025

};

struct NodoPedido {
    Pedido dato;
    NodoPedido* sig;
    NodoPedido(const Pedido& p) : dato(p), sig(nullptr) {}
};

class ListaPedidos {
private:
    NodoPedido* cabeza; // Puntero al primer pedido
    friend class ArbolLibrerias;
public:
    ListaPedidos();
    ~ListaPedidos();

    // Métodos principales para el menú
    void insertar(const Pedido& p); // Para insertar pedidos
    bool eliminar(string id_pedido); // Opción 5
    Pedido* buscar(string id_pedido); // Opción 4
    void mostrar() const; // Opción 3

    // Métodos auxiliares
    bool estaVacia() const;
    NodoPedido* getCabeza() const; // Para recorrer desde fuera si hace falta

    int numPedidos() const;
};

struct Libreria {
    int id_libreria;       // 3 cifras
    string localidad;
    ListaPedidos pedidos;  // lista de pedidos (inicialmente vacía)
};

struct NodoABB {
    Libreria info;
    NodoABB* izq;
    NodoABB* der;
    NodoABB(const Libreria& l) : info(l), izq(nullptr), der(nullptr) {}
};

// ABB de Librerías
class ArbolLibrerias {
private:
    NodoABB* raiz;
    NodoABB* insertarRec(NodoABB* nodo, const Libreria& l, bool& exito);
    NodoABB* borrarRec(NodoABB* nodo, int id, bool& borrado);
    NodoABB* minimo(NodoABB* nodo);
    void inOrderContarPedidos(NodoABB* nodo) const;
    NodoABB* buscarRec(NodoABB* nodo, int id) const;
    void destruirRec(NodoABB* nodo);
    void recorrerYBuscarPedido(NodoABB* nodo, const string& id_pedido, NodoABB*& encontrada, Pedido*& pfound);
    void estadisticasRec(NodoABB* nodo, int& libreriaMaxId, int& maxPedidos,
                         string& libroMasVendido, int& ventasLibroMax,
                         string& materiaMasVendida, int& ventasMateriaMax,
                         Pedido* bufferPedidos, int& bufCount, int bufMax) const;
    void recorrerYRepartir(NodoABB* nodo, Pedido pedidos[], int n);
public:
    ArbolLibrerias();
    ~ArbolLibrerias();
    bool insertar(const Libreria& l);
    bool borrar(int id_libreria);
    NodoABB* buscar(int id_libreria) const;
    void mostrarConteoPedidos() const; // muestra árbol (inorder) con nº pedidos por librería
    void mostrarTodosPedidosDe(int id_libreria) const;
    Pedido* buscarPedidoPorId(const string& id_pedido);
    bool extraerPedidoPorId(const string& id_pedido);
    bool moverPedido(const string& id_pedido, int id_destino);
    void generarYRepartirPedidosAleatorios(int N_PED);
    void estadisticas(); // muestra: libro más vendido, materia más exitosa, librería con más ventas
};

int aleatorioEntre(int a, int b); // inclusive
string generarIdLibreria(); // devuelve 3 cifras como string
int generarIdLibreriaInt(); // devuelve int 3 cifras
string generarIdPedido();
string generarCodLibro();
string generarMateria();
string generarLocalidad();
string generarFechaAleatoria(); // formato DD-MM-2025
Pedido generarPedidoAleatorio();

#endif
