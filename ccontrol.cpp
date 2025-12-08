#include "ccontrol.h"
#include <iomanip>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cctype>

#include <string>
#include <cctype>
using namespace std;

//---ZONA PARA GENERACION ALEATORIA DE RECURSOS

// Función auxiliar para generar números aleatorios (inclusive)
int aleatorioEntre(int a, int b) {
    return a + rand() % (b - a + 1);
}

// Función para elegir una localidad al azar de la lista oficial
string generarLocalidad() {
    int indice = rand() % N_LOCALIDADES;
    return LOCALIDADES[indice];
}

// Función para generar ID de librería (3 cifras, rellenando con ceros)
string generarIdLibreria() {
    int id = aleatorioEntre(0, 999);
    stringstream ss;
    ss << setfill('0') << setw(3) << id;
    return ss.str();
}

// Función para generar ID de Pedido (P + 5 cifras)
string generarIdPedido() {
    int num = aleatorioEntre(10000, 99999);
    return "P" + to_string(num);
}

string generarMateria() {
    return MATERIAS[rand() % N_MATERIAS];
}

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



// Función para limpiar: quita espacios y pasa a minúsculas
string limpiar(const string& s) {
    string r = "";
    for (char c : s) {
        if (isalnum((unsigned char)c)) {
            r += tolower((unsigned char)c);
        } else if ((unsigned char)c > 127) {
            // Dejamos pasar caracteres extendidos (tildes) por si acaso
            r += c;
        }
        // Ignoramos espacios
    }
    return r;
}

// Función auxiliar para buscar patrones (Huella Digital)
bool cumplePatron(const string& input, char letraInicio, const string& subcadena) {
    if (input.empty()) return false;
    // 1. Comprobar letra inicial (ej: M de Mostoles)
    if (input[0] != letraInicio) return false;
    // 2. Comprobar si contiene la parte segura (ej: ostoles)
    if (input.find(subcadena) == string::npos) return false;

    return true;
}

// VALIDADOR ROBUSTO (Ignora problemas de tildes)
bool localidadValida(string& loc) {
    string input = limpiar(loc);

    for (int i = 0; i < N_LOCALIDADES; ++i) {
        bool coincidencia = false;

        // 1. Comprobación Exacta (contra la versión limpia del sistema)
        //    Nota: limpiar("Móstoles") da "móstoles", así que coincide con input "mostoles"
        if (input == limpiar(LOCALIDADES[i])) {
            coincidencia = true;
        }
        // 2. Comprobación de Patrones (Huella Digital) para casos difíciles
        else if (i == 0 && cumplePatron(input, 'm', "stoles")) coincidencia = true;
        else if (i == 1 && cumplePatron(input, 'a', "lcal")) coincidencia = true;
        else if (i == 2 && cumplePatron(input, 'l', "legan")) coincidencia = true;
        else if (i == 5 && cumplePatron(input, 'a', "corc")) coincidencia = true; // Alcorcón
        else if (i == 6 && cumplePatron(input, 't', "rrej")) coincidencia = true; // Torrejón
        else if (i == 19 && cumplePatron(input, 't', "rescantos")) coincidencia = true;

        if (coincidencia) {
            // Si sabemos qué ciudad es, SOBREESCRIBIMOS lo que puso el usuario con el nombre oficial del array (que tiene tildes y mayúsculas bien puestas)
            loc = LOCALIDADES[i];
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
void ListaPedidos::insertar(const Pedido& p) {
    NodoPedido* nuevo = new NodoPedido(p);
    nuevo->sig = cabeza;
    cabeza = nuevo;
}
//Implementación de ArbolLibrerias

bool ListaPedidos::eliminar(string id_pedido) {
    // 1. Si la lista está vacía, nada
    if (cabeza == nullptr) return false;

    // 2. Caso especial: El pedido a borrar es el PRIMERO (cabeza)
    if (cabeza->dato.id_pedido == id_pedido) {
        NodoPedido* borrar = cabeza;
        cabeza = cabeza->sig; // Saltamos al siguiente
        delete borrar;        // Liberamos memoria
        return true;
    }

    // 3. Caso normal: Buscar en el resto de la lista
    NodoPedido* actual = cabeza;
    while (actual->sig != nullptr) {
        // Miramos "el siguiente" sin movernos todavía
        if (actual->sig->dato.id_pedido == id_pedido) {
            NodoPedido* borrar = actual->sig;

            // "Puenteamos": El actual apunta al siguiente del borrado
            actual->sig = borrar->sig;

            delete borrar;
            return true;
        }
        actual = actual->sig; // Avanzamos
    }

    return false; // No lo encontramos
}

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
void ArbolLibrerias::mostrarIdsLibrerias() const {
    mostrarIdsRec(raiz);
}

// Función recursiva auxiliar
void ArbolLibrerias::mostrarIdsRec(NodoABB* nodo) const {
    if (nodo != nullptr) {
        // Recorrer subárbol izquierdo
        mostrarIdsRec(nodo->izq);

        // Procesar el nodo actual
        cout << "ID Libreria: " << nodo->info.id_libreria << endl;

        // Recorrer subárbol derecho
        mostrarIdsRec(nodo->der);
    }
}
// --- LÓGICA DE INSERCIÓN (Opción 1) ---

// Función pública:
bool ArbolLibrerias::insertar(const Libreria& l) {
    bool exito = false; // Inicialmente asumimos que fallará
    raiz = insertarRec(raiz, l, exito);
    return exito; // Devolvemos el resultado al main
}
Pedido* ArbolLibrerias::buscarPedidoPorId(const string& id_pedido) {
    return buscarPedidoPorIdRec(raiz, id_pedido);
}
Pedido* ArbolLibrerias::buscarPedidoPorIdRec(NodoABB* nodo, const string& id_pedido) {
    if (nodo == nullptr) return nullptr;

    // Buscar en la lista de pedidos de esta librería
    Pedido* p = nodo->info.pedidos.buscarPorId(id_pedido);
    if (p != nullptr) {
        return p; // Encontrado
    }

    // Buscar en el subárbol izquierdo
    p = buscarPedidoPorIdRec(nodo->izq, id_pedido);
    if (p != nullptr) return p;

    // Buscar en el subárbol derecho
    return buscarPedidoPorIdRec(nodo->der, id_pedido);
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

        // 3. importante:
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
void generarPedidoAleatorio(ArbolLibrerias& editorial, int n_pedidos) {

    // Pedimos al usuario que elija una librería EXISTENTE
    int idLib;
    cout << "Introduzca el ID de la libreria donde desea generar los pedidos: ";
    cin >> idLib;

    // Verificamos que exista
    if (editorial.buscar(idLib) == nullptr) {
        cout << ">>> Error: La libreria con ID " << idLib << " no existe.\n";
        return;
    }

    cout << "Creando los siguientes pedidos nuevos:\n\n";
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
        string idPedido = "P" + to_string(20000 + rand() % 80000);
        string codLibro = generarCodLibro();
        string materia = materias[rand() % 6];
        int unidades = 5 + rand() % 20;
        string fecha = generarFechaAleatoria();

        // Insertamos EN ESA librería (que ya sabemos que existe)
        editorial.insertarPedidoEnLibreria(idLib, idPedido, codLibro, materia, unidades, fecha);

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
Pedido* ListaPedidos::buscarPorId(const string& id_pedido) {
    NodoPedido* aux = cabeza;

    while (aux != nullptr) {
        if (aux->dato.id_pedido == id_pedido) {
            return &(aux->dato);  // Devolvemos la dirección del pedido encontrado
        }
        aux = aux->sig;
    }
    return nullptr; // No encontrado
}

// Función pública
bool ArbolLibrerias::extraerPedidoPorId(const string& id_pedido) {
    return borrarPedidoRec(raiz, id_pedido);
}

// Función recursiva (Añadiremos su prototipo al .h en el paso 3)
bool ArbolLibrerias::borrarPedidoRec(NodoABB* nodo, const string& id_pedido) {
    if (nodo == nullptr) return false;

    // 1. Intentamos borrarlo de ESTA librería
    // Si eliminar() devuelve true, es que estaba aquí y ya lo ha borrado. ¡Trabajo hecho!
    if (nodo->info.pedidos.eliminar(id_pedido)) {
        cout << ">>> Pedido eliminado de la libreria: " << nodo->info.id_libreria << endl;
        return true;
    }

    // 2. Si no, probamos izquierda
    if (borrarPedidoRec(nodo->izq, id_pedido)) return true;

    // 3. Si no, probamos derecha
    return borrarPedidoRec(nodo->der, id_pedido);
}

//Iniciar el programa con 10 librerias
void ArbolLibrerias::generarLibreriasInicio(int n) {
    cout << "Creado el ABB con " << n << " nodos:" << endl;

    int insertadas = 0;
    while (insertadas < n) {
        Libreria l;
        // Generamos ID aleatorio (entre 100 y 999 para que sean 3 cifras)
        l.id_libreria = aleatorioEntre(100, 999);
        l.localidad = generarLocalidad();

        // Intentamos insertar. Si devuelve true (éxito), sumamos 1.
        // Si devuelve false (ya existía el ID), el bucle repite sin sumar.
        if (insertar(l)) {
            insertadas++;
        }
    }
}


// Funciones auxiliares locales para recolectar IDs válidos
int auxiliarContarNodos(NodoABB* nodo) {
    if (nodo == nullptr) return 0;
    return 1 + auxiliarContarNodos(nodo->izq) + auxiliarContarNodos(nodo->der);
}

void auxiliarGuardarIds(NodoABB* nodo, int* lista, int& indice) {
    if (nodo == nullptr) return;
    // Guardamos el ID de esta librería
    lista[indice] = nodo->info.id_libreria;
    indice++;
    // Seguimos recorriendo
    auxiliarGuardarIds(nodo->izq, lista, indice);
    auxiliarGuardarIds(nodo->der, lista, indice);
}

// Función auxiliar necesaria para el reparto
bool ArbolLibrerias::insertarPedidoEnLibreria(int id_libreria, const string& id_pedido, const string& cod_libro, const string& materia, int unidades, const string& fecha) {
    NodoABB* nodo = buscar(id_libreria);
    if (nodo != nullptr) {
        Pedido p;
        p.id_libreria = id_libreria;
        p.id_pedido = id_pedido;
        p.cod_libro = cod_libro;
        p.materia = materia;
        p.unidades = unidades;
        p.fecha_envio = fecha;
        nodo->info.pedidos.insertar(p);
        return true;
    }
    return false;
}

void ArbolLibrerias::generarYRepartirPedidosAleatorios(int n_pedidos) {
    // 1. PREPARACIÓN: Obtener los IDs REALES que existen en el árbol
    int totalLibrerias = auxiliarContarNodos(raiz);

    if (totalLibrerias == 0) {
        cout << ">>> Error: No hay librerias en el sistema. Genere librerias primero (Opcion 1 o Inicio)." << endl;
        return;
    }

    // Creamos una lista temporal con los IDs válidos (ej: 100, 240, 890...)
    int* idsValidos = new int[totalLibrerias];
    int contador = 0;
    auxiliarGuardarIds(raiz, idsValidos, contador);


    // 2. GENERACIÓN
    cout << "\nGenerando " << n_pedidos << " pedidos aleatorios..." << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
    cout << "| ID Lib | ID Pedido | Cod Libro | Materia       | Unid | Fecha      |" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;

    struct PedidoTemp {
        int idLib;
        string idPed;
        string cod;
        string mat;
        int unid;
        string fec;
    };

    PedidoTemp* lote = new PedidoTemp[n_pedidos];

    for (int i = 0; i < n_pedidos; i++) {
        int indiceAzar = rand() % totalLibrerias;
        lote[i].idLib = idsValidos[indiceAzar];

        lote[i].idPed = generarIdPedido();
        lote[i].cod = generarCodLibro();
        lote[i].mat = generarMateria();
        lote[i].unid = aleatorioEntre(1, 20);
        lote[i].fec = generarFechaAleatoria();

        cout << "| " << setw(7) << lote[i].idLib
             << "| " << setw(10) << lote[i].idPed
             << "| " << setw(10) << lote[i].cod
             << "| " << left << setw(14) << lote[i].mat
             << "| " << right << setw(5) << lote[i].unid
             << "| " << setw(11) << lote[i].fec << "|" << endl;
    }
    cout << "-------------------------------------------------------------------------------" << endl;

    cout << "\nPresione ENTER para distribuir los pedidos en las librerias...";
    //cin.ignore();
    cin.get();

    // 3. REPARTO
    int exitos = 0;
    int fallos = 0;

    for (int i = 0; i < n_pedidos; i++) {
        if (insertarPedidoEnLibreria(lote[i].idLib, lote[i].idPed, lote[i].cod, lote[i].mat, lote[i].unid, lote[i].fec)) {
            exitos++;
        } else {
            fallos++;
        }
    }

    cout << "\n--- REPARTO FINALIZADO ---" << endl;
    cout << "Pedidos asignados correctamente: " << exitos << endl;
    cout << "Pedidos perdidos (Libreria no existe): " << fallos << endl;

    // Limpiamos memoria dinámica
    delete[] lote;
    delete[] idsValidos;
}

// IMPLEMENTACIÓN OPCIÓN 6: MOVER PEDIDO

bool ArbolLibrerias::moverPedido(const string& id_pedido, int id_destino) {
    // Verificar si la librería de destino existe
    NodoABB* nodoDestino = buscar(id_destino);
    if (nodoDestino == nullptr) {
        cout << ">>> Error: La libreria de destino (" << id_destino << ") no existe." << endl;
        return false;
    }

    // Buscar el pedido para ver si existe y obtener sus datos
    Pedido* p = buscarPedidoPorId(id_pedido);
    if (p == nullptr) {
        cout << ">>> Error: El pedido " << id_pedido << " no existe en el sistema." << endl;
        return false;
    }

    // Verificar si ya está en esa librería
    if (p->id_libreria == id_destino) {
        cout << ">>> Aviso: El pedido ya se encuentra en la libreria " << id_destino << "." << endl;
        return false;
    }

    // HACER UNA COPIA DE SEGURIDAD DEL PEDIDO
    // Es vital porque 'extraerPedidoPorId' va a liberar la memoria del original
    Pedido copiaPedido = *p;

    // Actualizamos el ID de librería en la copia (ahora pertenece a la nueva)
    copiaPedido.id_libreria = id_destino;

    // Borramos el pedido de la librería original (Origen).Sabemos que existe, así que debería devolver true
    if (extraerPedidoPorId(id_pedido)) {

        // Insertamos la copia en la librería de destino
        nodoDestino->info.pedidos.insertar(copiaPedido);

        cout << ">>> Traslado completado: Pedido movido a " << nodoDestino->info.localidad << "." << endl;
        return true;
    }

    return false; // Fallo inesperado al borrar
}

// IMPLEMENTACIÓN OPCIÓN 7: ESTADÍSTICAS

// Función principal que calcula y muestra los datos
void ArbolLibrerias::estadisticas() {
    // 1. Preparación de variables
    int libreriaMaxId = -1;
    int maxPedidos = -1;

    // Buffer temporal para "sacar" todos los pedidos del árbol y analizarlos juntos
    // (Asumimos un máximo seguro de 1000 pedidos para esta práctica)
    int bufMax = 1000;
    Pedido* bufferPedidos = new Pedido[bufMax];
    int bufCount = 0; // Cuántos pedidos hemos guardado realmente

    // Variables basura para pasar por referencia (se calcularán después de llenar el buffer)
    string strFicticio;
    int intFicticio;

    cout << "\nCalculando estadisticas globales del sistema..." << endl;

    // 2. LLAMADA RECURSIVA:
    // - Recorre el árbol.
    // - Encuentra la librería con más pedidos.
    // - Llena el bufferPedidos con TODOS los pedidos de TODAS las librerías.
    estadisticasRec(raiz, libreriaMaxId, maxPedidos, strFicticio, intFicticio, strFicticio, intFicticio, bufferPedidos, bufCount, bufMax);

    // 3. PROCESAMIENTO DE DATOS
    if (bufCount == 0) {
        cout << ">>> No hay datos suficientes (Sin pedidos en el sistema)." << endl;
        delete[] bufferPedidos;
        return;
    }

    // A) Calcular Materia Más Exitosa
    // Usamos un array de contadores alineado con el array global MATERIAS[]
    int conteoMaterias[N_MATERIAS] = {0}; // Inicializados a 0

    for (int i = 0; i < bufCount; i++) {
        // Buscamos a qué materia corresponde este pedido
        for (int j = 0; j < N_MATERIAS; j++) {
            if (bufferPedidos[i].materia == MATERIAS[j]) {
                conteoMaterias[j] += bufferPedidos[i].unidades; // Sumamos las unidades vendidas
                break;
            }
        }
    }

    // Buscamos el ganador
    string materiaMasVendida = "N/A";
    int ventasMateriaMax = -1;

    for (int j = 0; j < N_MATERIAS; j++) {
        if (conteoMaterias[j] > ventasMateriaMax) {
            ventasMateriaMax = conteoMaterias[j];
            materiaMasVendida = MATERIAS[j];
        }
    }

    // B) Calcular Libro Más Vendido
    // Estrategia: Doble bucle para contar frecuencias
    string libroMasVendido = "N/A";
    int ventasLibroMax = -1;

    // Array para no contar el mismo libro dos veces
    bool* procesado = new bool[bufCount];
    for(int i=0; i<bufCount; i++) procesado[i] = false;

    for (int i = 0; i < bufCount; i++) {
        if (procesado[i]) continue; // Si ya lo contamos, saltar

        string codigoActual = bufferPedidos[i].cod_libro;
        int ventasActuales = 0;

        // Contamos cuántas unidades suma este código en TODO el buffer
        for (int j = i; j < bufCount; j++) {
            if (bufferPedidos[j].cod_libro == codigoActual) {
                ventasActuales += bufferPedidos[j].unidades;
                procesado[j] = true; // Marcamos para no volver a mirarlo
            }
        }

        // ¿Es el nuevo récord?
        if (ventasActuales > ventasLibroMax) {
            ventasLibroMax = ventasActuales;
            libroMasVendido = codigoActual;
        }
    }
    delete[] procesado;

    // 4. MOSTRAR RESULTADOS FINALES
    cout << "\n========================================" << endl;
    cout << "       INFORME DE ESTADISTICAS" << endl;
    cout << "========================================" << endl;

    cout << "1. LIBRERIA CON MAS VENTAS (Mayor N. Pedidos):" << endl;
    if (libreriaMaxId != -1) {
        NodoABB* topLib = buscar(libreriaMaxId);
        cout << "   ID: " << libreriaMaxId;
        if (topLib) cout << " (" << topLib->info.localidad << ")";
        cout << "\n   Total Pedidos: " << maxPedidos << endl;
    }

    cout << "\n2. LIBRO MAS VENDIDO:" << endl;
    cout << "   Codigo: " << libroMasVendido << endl;
    cout << "   Unidades totales: " << ventasLibroMax << endl;

    cout << "\n3. MATERIA MAS EXITOSA:" << endl;
    cout << "   Area: " << materiaMasVendida << endl;
    cout << "   Unidades totales: " << ventasMateriaMax << endl;

    cout << "========================================" << endl;

    // Limpieza de memoria
    delete[] bufferPedidos;
}

// Función Recursiva: Recorre el árbol para llenar el buffer y buscar la librería top
void ArbolLibrerias::estadisticasRec(NodoABB* nodo, int& libreriaMaxId, int& maxPedidos,
                                     string& libroMasVendido, int& ventasLibroMax,
                                     string& materiaMasVendida, int& ventasMateriaMax,
                                     Pedido* bufferPedidos, int& bufCount, int bufMax) const {
    if (nodo == nullptr) return;

    // 1. Análisis Local (Librería actual)
    // Miramos si esta librería tiene más pedidos que el récord actual
    int nPeds = nodo->info.pedidos.numPedidos();
    if (nPeds > maxPedidos) {
        maxPedidos = nPeds;
        libreriaMaxId = nodo->info.id_libreria;
    }

    // 2. Volcado de datos (Sacamos los pedidos de la lista y los metemos al buffer)
    NodoPedido* aux = nodo->info.pedidos.getCabeza();
    while (aux != nullptr) {
        if (bufCount < bufMax) {
            bufferPedidos[bufCount] = aux->dato; // Copia del pedido al array gigante
            bufCount++;
        }
        aux = aux->sig;
    }

    // 3. Recursividad (Seguir recorriendo el árbol)
    estadisticasRec(nodo->izq, libreriaMaxId, maxPedidos, libroMasVendido, ventasLibroMax,
                    materiaMasVendida, ventasMateriaMax, bufferPedidos, bufCount, bufMax);
    estadisticasRec(nodo->der, libreriaMaxId, maxPedidos, libroMasVendido, ventasLibroMax,
                    materiaMasVendida, ventasMateriaMax, bufferPedidos, bufCount, bufMax);
}

// Getter necesario para acceder a la cabeza de la lista desde la clase Arbol
NodoPedido* ListaPedidos::getCabeza() const {
    return cabeza;
}
