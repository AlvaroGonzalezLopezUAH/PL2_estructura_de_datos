#include <iostream>
#include "ccontrol.h"

using namespace std;

int main() {
    // Creamos el árbol (vacío al inicio)
    ArbolLibrerias editorial;

    int opcion;

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
        cout << "0- Salir." << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: {
                // OPCION 1: INSERTAR
                Libreria nuevaLib;
                cout << "Introduzca ID de la libreria (3 cifras): ";
                cin >> nuevaLib.id_libreria;
                cout << "Introduzca Localidad: ";
                cin >> nuevaLib.localidad;

                // USAMOS EL IF PARA VERIFICAR
                if (editorial.insertar(nuevaLib)) {
                    cout << ">>> Exito: Libreria insertada correctamente." << endl;
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
            case 4:
                // Pendiente...
                break;
            case 5:
                // Pendiente...
                break;
            case 6:
                // Pendiente...
                break;
            case 7:
                //no está completa, por ahora muestra una lista simple para ver que se estan creando las librerias correctamente
                editorial.mostrarConteoPedidos();
                break;
            case 8:
                // Pendiente...
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
