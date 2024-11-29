#include <iostream>
#include <ctime>
#include <limits>
#include <iomanip>
#include <vector>
#include <string>
#include <sqlite3.h>
#include "CrearCuenta.hpp"
#include "Verificacion.hpp" 
#include "AtencionCliente.hpp"
#include "baseDatos.hpp"
#include "UU.hpp"
#include "InformacionPrestamos.hpp"

using namespace std;

/**
 * @brief Función principal del sistema bancario.
 * 
 * Esta función es el punto de entrada del programa, que presenta un menú interactivo en la consola donde el usuario puede 
 * elegir entre tres opciones: ingresar al sistema, crear una cuenta o salir del programa. La opción seleccionada ejecuta 
 * una serie de operaciones, como la verificación de una cuenta bancaria o la creación de una cuenta nueva. Además, el 
 * sistema muestra información sobre el tipo de cambio actual de CRC a USD.
 * 
 * @return int Código de salida del programa.
 */
int main() {
    int option;
    double TipoCambioMin = 520.0;
    double TipoCambioMax = 537.5;
    double TipoCambio = generarTipoCambio(TipoCambioMin, TipoCambioMax);

    // Abrir la base de datos
    sqlite3* db = abrirBaseDatos("SistemaBancario.db");
    crearTablas(db);

    do {
        try{
            cout << "-----------------------------------------------------------------------------" << endl;
            cout << "|                     Bienvenido al Sistema en ventanilla                   |" << endl;
            cout << "-----------------------------------------------------------------------------" << endl;
            cout << "|                                      |                                    |" << endl;
            cout << "|                                      |                                    |" << endl;
            cout << "|        1. Ingresar al Sistema        |        2. Crear una Cuenta         |" << endl;
            cout << "|                                      |                                    |" << endl;
            cout << "|                                      |                                    |" << endl;
            cout << "-----------------------------------------------------------------------------" << endl;
            cout << "|                                                                           |" << endl;
            cout << "|                               Tipo de cambio:                             |" << endl;
            cout << "|                               " << TipoCambio << "  CRC/USD                             |" << endl;
            cout << "|---------------------------------------------------------------------------|" << endl;
            cout << "|                             3.Salir del sistema                           |" << endl;
            cout << "|---------------------------------------------------------------------------|" << endl;
            cout << "Ingrese su selección: " << endl; 
            cin >> option;
            // Comprobamos si hubo un error de entrada
            if (cin.fail() || option < 1 || option > 3) {
                throw invalid_argument("Opción inválida. Debe ingresar un número entre 1 y 3.");
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Limpiar el buffer
        } catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;

            // Limpiar el estado de error de cin y descartar la entrada restante
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Limpiar el buffer

            continue;  // Volver a mostrar el menú si la opción es inválida
        }

        switch (option) {
            case 1: {
                int id;
                string password;
                try {
                    // Validar ID de cuenta
                    cout << "Ingrese el ID de la cuenta: ";
                    cin >> id;
                    if (cin.fail()) {
                        throw invalid_argument("ID no válido. Debe ser un número entero.");
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Limpiar el buffer

                    // Validar contraseña
                    cout << "Ingrese su Password: ";
                    getline(cin, password);
                    if (password.empty()) {
                        throw invalid_argument("La contraseña no puede estar vacía.");
                    }

                    if (!verificarCuenta(db, id, password)) {
                        cout << "Cuenta no encontrada. Por favor intente de nuevo." << endl;
                        break; // Volver al menú
                    }


                    int opcion1;
                
                    do {
                        cout << "Ingrese el servicio que desea recibir: " << endl;
                        cout << "1. Atención al cliente.\n2. Información general sobre Préstamos.\n3. Devolverse" << endl;

                        cout << "Ingrese su selección: ";
                        cin >> opcion1;
                        if (cin.fail()) {
                            throw invalid_argument("Opción inválida. Debe ingresar un número.");
                        }
                        
                        switch (opcion1) {

                            case 1:
                                cout << "Ingresando al menú de atención al cliente..." << endl;
                                showMenuAC(id, TipoCambio, db);
                                break;

                            case 2:
                                cout << "Ingresando al menú de solicitud de prestamo..." << endl;
                                showMenuSP(id, db);
                                break;

                            case 3:
                                cout << "\nAquí se devuelve al menú anterior" << endl;
                                break;

                            default:
                                cout << "\nOpción inválida...\nPor favor intente de nuevo.\n" << endl;
                        }
                    } while (opcion1 != 3);
                    break;
                } catch (const invalid_argument& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 2:
                gestionarCreacionCuenta(db);
                cout << "\nConsultando las tablas después de crear la cuenta:\n";
                cout << "\nTabla Cuenta_Colones:" << endl;
                consultarTabla(db, "Cuenta_Colones");
                cout << "\nTabla Cuenta_Dolares:" << endl;
                consultarTabla(db, "Cuenta_Dolares");
                break;
            case 3:
                cout << "\nSaliendo del programa...\nGracias por su visita." << endl;
                

        }

    } while (option != 3);

    sqlite3_close(db);
    return 0;
}




 
/* Funcionamiento para no perderme al hacer los menús xD
 Agregar los otros displays
 Para el caso de que se ingrese la opcion 1: Ingresar al sistema 
___________________________________________________________________________________________________
 Caso ingresar al sistema: 
    1. Atención cliente:
        - Despliega otro menú en el cual se pueden ver todas las opciones de atencion al cliente
    2. Información general de Préstamos:
        - Despliega el menu con las opciones establecidas para este.
___________________________________________________________________________________________________
 Caso de crear una cuenta: 
 -> Despliega menú con los requerimientos 
    1. Nombre: (Recibe)
    2. Tipo de cuente: (Recibe la opción)
        - 1. Colones 
        - 2. Dólares
        - 3. Ambas
    -> Imprime el id de cuenta 
    -> Imprimir el cvv 
    -> Imprimir el PIN
___________________________________________________________________________________________________
*/
