#include "ccontrol.h"
#include <iomanip>
#include <sstream>
#include <cstring>
#include <algorithm>
//Permite que el cliente escriba sin tildes ni mayusculas
string normalizar(const string& s) {
    string r;
    r.reserve(s.size());
    for (char c : s) {
        // Convertimos a minúscula
        char x = tolower((unsigned char)c);

        // Quitamos tildes
        switch (x) {
            case 'á': x = 'a'; break;
            case 'é': x = 'e'; break;
            case 'í': x = 'i'; break;
            case 'ó': x = 'o'; break;
            case 'ú': x = 'u'; break;
        }
        r.push_back(x);
    }
    return r;
}
//Logica para que las ciudades introducidas en la funcion 1 esten definidas
bool localidadValida(const string& loc) {
    const string LOCALIDADES[] = {
        "Móstoles","Alcalá","Leganés","Fuenlabrada","Getafe","Alcorcón","Torrejón","Parla",
        "Alcobendas","Coslada","Pozuelo","Rivas","Valdemoro","Majadahonda","Aranjuez","Arganda",
        "Boadilla","Pinto","Colmenar","Tres Cantos"
    };
    const int N = sizeof(LOCALIDADES) / sizeof(LOCALIDADES[0]);

    string locNorm = normalizar(loc);

    for (int i = 0; i < N; ++i) {
        if (locNorm == normalizar(LOCALIDADES[i])) {
            return true;
        }
    }
    return false;
}
ListaPedidos::ListaPedidos() : cabeza(nullptr) {
    // Inicializamos la lista vacía
}

ListaPedidos::~ListaPedidos() {
    // Destructor: Borra todos los nodos para liberar memoria
    NodoPedido* aux = cabeza;
    while (aux != nullptr) {
        NodoPedido* borrar = aux;
        aux = aux->sig;
        delete borrar;
    }
}

//Implementación de ArbolLibrerias

ArbolLibrerias::ArbolLibrerias() : raiz(nullptr) {
    // Inicializamos el árbol vacío
}

ArbolLibrerias::~ArbolLibrerias() {
    destruirRec(raiz);
}

void ArbolLibrerias::destruirRec(NodoABB* nodo) {
    if (nodo != nullptr) {
        destruirRec(nodo->izq);
        destruirRec(nodo->der);
        delete nodo;
    }
}

// --- LÓGICA DE INSERCIÓN (Opción 1) ---

// Función pública:
bool ArbolLibrerias::insertar(const Libreria& l) {
    bool exito = false; // Inicialmente asumimos que fallará
    raiz = insertarRec(raiz, l, exito);
    return exito; // Devolvemos el resultado al main
}

// Función privada:
NodoABB* ArbolLibrerias::insertarRec(NodoABB* nodo, const Libreria& l, bool& exito) {
    // 1. CASO BASE: Hueco encontrado -> ÉXITO
    if (nodo == nullptr) {
        exito = true; // Marcamos que hemos insertado
        return new NodoABB(l);
    }

    // 2. RECURSIVIDAD
    if (l.id_libreria < nodo->info.id_libreria) {
        nodo->izq = insertarRec(nodo->izq, l, exito);
    }
    else if (l.id_libreria > nodo->info.id_libreria) {
        nodo->der = insertarRec(nodo->der, l, exito);
    }
    else {
        // 3. CASO DUPLICADO: l.id == nodo->id
        // No hacemos nada. 'exito' se queda en false (su valor original).
        // El nodo se devuelve tal cual.
    }
    return nodo;
}


// 1. Método auxiliar para contar pedidos en la lista
int ListaPedidos::numPedidos() const {
    int cont = 0;
    NodoPedido* aux = cabeza;
    while (aux != nullptr) {
        cont++;
        aux = aux->sig;
    }
    return cont;
}

// 2. Función pública para mostrar el árbol (Llama a la recursiva)
void ArbolLibrerias::mostrarConteoPedidos() const {
    if (raiz == nullptr) {
        cout << "El arbol esta vacio." << endl;
    } else {
        cout << "\n--- LISTADO DE LIBRERIAS (Ordenado por ID) ---" << endl;
        inOrderContarPedidos(raiz);
        cout << "----------------------------------------------" << endl;
    }
}

// 3. Función recursiva (In-Order: Izq -> Info -> Der)
void ArbolLibrerias::inOrderContarPedidos(NodoABB* nodo) const {
    if (nodo != nullptr) {
        // 1. Recorrer izquierda (menores)
        inOrderContarPedidos(nodo->izq);

        // 2. Procesar el nodo actual (Raíz del subárbol)
        cout << "ID: " << nodo->info.id_libreria
             << " \tLocalidad: " << left << setw(15) << nodo->info.localidad
             << " Num Pedidos: " << nodo->info.pedidos.numPedidos()
             << endl;

        // 3. Recorrer derecha (mayores)
        inOrderContarPedidos(nodo->der);
    };


}

// --- LÓGICA DE BORRADO (Opción 2) ---

// Función auxiliar para encontrar el nodo más pequeño de una rama
NodoABB* ArbolLibrerias::minimo(NodoABB* nodo) {
    if (nodo == nullptr) return nullptr;
    while (nodo->izq != nullptr) {
        nodo = nodo->izq;
    }
    return nodo;
}

// Función pública
bool ArbolLibrerias::borrar(int id_libreria) {
    bool borrado = false;
    raiz = borrarRec(raiz, id_libreria, borrado);
    return borrado;
}

// Función recursiva compleja
NodoABB* ArbolLibrerias::borrarRec(NodoABB* nodo, int id, bool& borrado) {
    // 1. Caso Base: Árbol vacío o no encontrado
    if (nodo == nullptr) return nullptr;

    // 2. Buscar el nodo
    if (id < nodo->info.id_libreria) {
        nodo->izq = borrarRec(nodo->izq, id, borrado);
    }
    else if (id > nodo->info.id_libreria) {
        nodo->der = borrarRec(nodo->der, id, borrado);
    }
    else {
        // Lo encuentra (id == nodo->info.id)
        borrado = true;

        // CASO A: Sin hijos o solo hijo derecho
        if (nodo->izq == nullptr) {
            NodoABB* temp = nodo->der;
            delete nodo; // El destructor de Libreria -> ListaPedidos se encarga de la memoria
            return temp;
        }
        // CASO B: Solo hijo izquierdo
        else if (nodo->der == nullptr) {
            NodoABB* temp = nodo->izq;
            delete nodo;
            return temp;
        }

        // CASO C: Dos hijos
        // 1. Buscamos el mínimo del subárbol derecho (el sucesor)
        NodoABB* temp = minimo(nodo->der);

        // 2. Copiamos TODA la info del sucesor al nodo actual (salvamos los datos)
        // OJO: Esto hace una copia superficial de la lista (copia el puntero 'cabeza')
        nodo->info = temp->info;

        // 3. TRUCO DE INGENIERÍA:
        // Como 'temp' se va a borrar abajo, su destructor querrá borrar la lista de pedidos.
        // Pero esa lista ahora pertenece a 'nodo'. Debemos evitar que 'temp' la destruya.
        // Accedemos a la parte privada gracias al 'friend class' y anulamos el puntero del original.
        temp->info.pedidos.cabeza = nullptr;

        // 4. Borramos al sucesor original (que ahora está duplicado)
        nodo->der = borrarRec(nodo->der, temp->info.id_libreria, borrado);
    }
    return nodo;
}

// --- LÓGICA PARA LA OPCION 3 (MOSTRAR LOS PEDIDOS DE UNA LIB)

// Función pública de búsqueda
NodoABB* ArbolLibrerias::buscar(int id_libreria) const {
    return buscarRec(raiz, id_libreria);
}

// Función recursiva de búsqueda
NodoABB* ArbolLibrerias::buscarRec(NodoABB* nodo, int id) const {
    if (nodo == nullptr) {
        return nullptr; // No encontrado (hemos llegado al final de una rama)
    }

    if (id == nodo->info.id_libreria) {
        return nodo; // Lo encuentra
    }

    if (id < nodo->info.id_libreria) {
        return buscarRec(nodo->izq, id); // Buscar a la izquierda
    } else {
        return buscarRec(nodo->der, id); // Buscar a la derecha
    }
}

void ListaPedidos::mostrar() const {
    if (cabeza == nullptr) {
        cout << "   (No hay pedidos registrados en esta libreria)" << endl;
        return;
    }

    cout << "   Listado de pedidos:" << endl;
    cout << "   --------------------------------------------------------------------------------" << endl;
    cout << "   |ID Pedido | Cod Libro | Materia      | Unid | Fecha      |" << endl;
    cout << "   --------------------------------------------------------------------------------" << endl;

    NodoPedido* aux = cabeza;
    while (aux != nullptr) {
        cout << "   | " << left << setw(9) << aux->dato.id_pedido
             << "| " << setw(10) << aux->dato.cod_libro
             << "| " << setw(13) << aux->dato.materia
             << "| " << setw(5) << aux->dato.unidades
             << "| " << setw(11) << aux->dato.fecha_envio << "|" << endl;
        aux = aux->sig;
    }
    cout << "   --------------------------------------------------------------------------------" << endl;
}

void ArbolLibrerias::mostrarTodosPedidosDe(int id_libreria) const {
    // 1. Buscamos la librería en el árbol
    NodoABB* nodo = buscar(id_libreria);

    if (nodo == nullptr) {
        cout << "Error: La libreria con ID " << id_libreria << " no existe en el sistema." << endl;
    } else {
        // 2. Si existe, mostramos sus datos generales
        cout << "\n--- INFORMACION DE LIBRERIA " << id_libreria << " ---" << endl;
        cout << "Localidad: " << nodo->info.localidad << endl;

        // 3. Delegamos en la lista para que se muestre a sí misma
        nodo->info.pedidos.mostrar();
    }
}
// ---LOGICA PARA LA OPCION 8 (crear n_pedidos)
void generarPedidoAleatorio(int n_pedidos) {
    cout << "Creando los siguientes pedidos nuevos:\n\n";

    // Encabezado de la tabla
    cout << left
         << setw(12) << "ID Libreria"
         << setw(12) << "ID Pedido"
         << setw(12) << "Cod Libro"
         << setw(15) << "Materia"
         << setw(10) << "Unidades"
         << setw(12) << "Fecha"
         << "\n";
    cout << string(73, '-') << "\n";

    string materias[] = {"Matematicas", "Musica", "Fisica", "Lengua", "Historia", "Tecnologia"};

    for (int i = 0; i < n_pedidos; i++) {

        // Datos aleatorios
        int idLib = 500 + rand() % 400;
        string idPedido = "P" + to_string(20000 + rand() % 80000);
        string codLibro = generarCodLibro(); // Tu función
        string materia = materias[rand() % 6];
        int unidades = 5 + rand() % 20;
        string fecha = generarFechaAleatoria(); // Tu función

        // Mostrar tabla alineada
        cout << left
             << setw(12) << idLib
             << setw(12) << idPedido
             << setw(12) << codLibro
             << setw(15) << materia
             << setw(10) << unidades
             << setw(12) << fecha
             << "\n";
    }
}
//---ZONA PARA GENERACION ALEATORIA DE RECURSOS
string generarCodLibro() {
    string letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string numeros = "0123456789";
    string codigo = "";

    for (int i = 0; i < 3; i++)
        codigo += letras[rand() % letras.size()];

    for (int i = 0; i < 3; i++)
        codigo += numeros[rand() % numeros.size()];

    return codigo;
}
string generarFechaAleatoria() {
    int dia = 1 + rand() % 28;
    int mes = 1 + rand() % 12;

    return to_string(dia) + "-" + to_string(mes) + "-2025";
}

