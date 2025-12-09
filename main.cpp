#include <iostream>
#include "ccontrol.h"

using namespace std;
bool localidadValida(const std::string& loc);
int n_pedidos;
string id;
string letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main() {
    srand(time(nullptr));

    ArbolLibrerias editorial;
    int opcion;

    // FASE DE INICIO AUTOMÁTICO

    // 1. Generar 10 librerías aleatorias (N_LIBRERIAS)
    editorial.generarLibreriasInicio(N_LIBRERIAS);

    // 2. Mostrar estado inicial (deben salir con 0 pedidos)
    cout << "Arbol vacio creado:" << endl;
    editorial.mostrarConteoPedidos();

    cout << "\nPresione ENTER para generar los pedidos..." << endl;
    cin.ignore();

    // 3. Generar 30 pedidos, mostrarlos y repartirlos al pulsar Enter
    editorial.generarYRepartirPedidosAleatorios(N_PEDIDOS);

    // 4. Mostrar estado final (con los pedidos ya sumados)
    cout << "\nPedidos repartidos. Estado del arbol:" << endl;
    editorial.mostrarConteoPedidos();

    cout << "\nIniciando sistema de gestion..." << endl;
    cout << "Presione ENTER para ir al menu.";
    cin.get();

    // FIN FASE DE INICIO

    do {
        cout << "\n--- MENU CENTRO DE CONTROL ---" << endl;
        cout << "1- Insertar una libreria de forma manual." << endl;
        cout << "2- Borrar una libreria del arbol." << endl;
        cout << "3- Mostrar los datos de los pedidos de una libreria dada." << endl;
        cout << "4- Buscar un pedido concreto por su ID." << endl;
        cout << "5- Extraer un pedido concreto." << endl;
        cout << "6- Llevar un pedido concreto de una libreria a otra." << endl;
        cout << "7- Mostrar una estadistica de las librerias." << endl;
        cout << "8- Continuar con la distribucion de pedidos." << endl;
        cout << "9- Ver ABB en inorden." << endl;
        cout << "0- Salir." << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {

            case 1: {
                // OPCION 1: INSERTAR
                Libreria nuevaLib;

                // 1. Pedir ID
                cout << "Introduzca ID de la libreria (3 cifras): ";
                cin >> nuevaLib.id_libreria;

                if (cin.fail()) {
                cin.clear(); // Limpia el estado de error
                cin.ignore(1000, '\n'); // Descarta la entrada incorrecta
                cout << "Error: Solo se permiten numeros de 3 cifras.\n";
                }
                if (nuevaLib.id_libreria < 100 || nuevaLib.id_libreria>999){
                    cout<<"Numero de libreria no valido ";
                    break;
                }

                // 2. Limpieza del buffer (NECESARIO para que getline no salte)
                cin.ignore();

                // 3. Pedir Localidad en bucle hasta que sea válida
                bool locEsValida = false;
                do {
                    cout << "Introduzca Localidad (Ej: Móstoles, Tres Cantos...): ";

                    // Usamos getline para permitir espacios (Tres Cantos)
                    getline(cin, nuevaLib.localidad);

                    if (localidadValida(nuevaLib.localidad)) {
                        locEsValida = true;
                    } else {
                        cout << ">>> Error: Localidad no reconocida. Intente de nuevo." << endl;
                    }
                } while (!locEsValida);

                // 4. Intentar insertar en el árbol
                if (editorial.insertar(nuevaLib)) {
                    cout << "Libreria insertada correctamente." << endl;
                } else {
                    cout << ">>> Error: Ya existe una libreria con el ID " << nuevaLib.id_libreria << "." << endl;
                }
                break;
            }

            case 2:
                int idBorrar;
                cout << "Introduzca ID de la libreria a borrar: ";
                cin >> idBorrar;

                if (editorial.borrar(idBorrar)) {
                    cout << "Libreria " << idBorrar << " eliminada correctamente." << endl;
                } else {
                    cout << "Error: La libreria con ID " << idBorrar << " no existe." << endl;
                }
                break;

            case 3:
                int idBuscar;
                cout << "Introduzca ID de la libreria a consultar: ";
                cin >> idBuscar;
                editorial.mostrarTodosPedidosDe(idBuscar);
                break;

            case 4:{
                cout << "Introduce el ID del pedido: ";
                cin >> id;

                Pedido* p = editorial.buscarPedidoPorId(id);

                if (p != nullptr) {
                    cout << "\nPedido encontrado:" << endl;
                    cout << "ID Libreria: " << p->id_libreria << endl;
                    cout << "ID Pedido: " << p->id_pedido << endl;
                    cout << "Cod Libro: " << p->cod_libro << endl;
                    cout << "Materia: " << p->materia << endl;
                    cout << "Unidades: " << p->unidades << endl;
                    cout << "Fecha: " << p->fecha_envio << endl;
                } else {
                    cout << "Pedido no encontrado." << endl;
                }
                break;
            }

            case 5:{
                string idPed;
                cout << "\nIntroduzca el ID del pedido a extraer (borrar): ";
                cin >> idPed;

                if (editorial.extraerPedidoPorId(idPed)) {
                    cout << "Operacion realizada con exito." << endl;
                } else {
                    cout << ">>> Error: No se pudo encontrar el pedido con ID " << idPed << "." << endl;
                }
                break;}

            case 6:{
                string idPed;
                int idDest;

                cout << "\n--- MOVER PEDIDO ---" << endl;
                cout << "Introduzca el ID del pedido a mover: ";
                cin >> idPed;

                // Primero verificamos visualmente si existe
                Pedido* check = editorial.buscarPedidoPorId(idPed);
                if (check != nullptr) {
                    cout << "El pedido esta actualmente en la libreria ID: " << check->id_libreria << endl;

                    cout << "Introduzca el ID de la libreria de DESTINO: ";
                    cin >> idDest;

                    if (editorial.moverPedido(idPed, idDest)) {
                        cout << "Operacion realizada con exito." << endl;
                    } else {
                        cout << "No se pudo mover el pedido." << endl;
                    }
                } else {
                    cout << ">>> Error: Pedido no encontrado." << endl;
                }
                break;}

            case 7:
                editorial.estadisticas();
                break;

            case 8:
            cout << "=== GENERADOR AUTOMATICO DE PEDIDOS ===" << endl;
            cout << "¿Cuántos pedidos nuevos desea generar? ";
            cin >> n_pedidos;

            if (n_pedidos < 0) {
                cout << ">>> Error: El número de pedidos debe ser >= 0." << endl;
                return 1;
            }
            cout << "Generando " << n_pedidos << " pedido(s)...\n" << endl;
            generarPedidoAleatorio(editorial, n_pedidos);
            cout << "\n>>> Proceso finalizado." << endl;
            break;

            case 9:
                editorial.mostrarConteoPedidos();
                break;

            case 0:
                cout << "Saliendo del programa..." << endl;
                break;

            default:
                cout << "Opcion no valida." << endl;
        }

    } while (opcion != 0);

    return 0;
}
