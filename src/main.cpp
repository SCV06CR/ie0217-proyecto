#include <iostream>
#include <ctime>
#include <limits>
#include <iomanip>
#include <vector>
#include <string>
#include <sqlite3.h>
#include "TipoCambio.hpp"
#include "AbrirDB.hpp"
#include "CrearCuenta.hpp"
#include "Verificacion.hpp" 
#include "ConsultaDB.hpp"
#include "AtencionCliente.hpp"
#include "baseDatos.hpp"
#include "InformacionPrestamos.hpp"

using namespace std;

int main() {
    int option;
    double TipoCambioMin = 520.0;
    double TipoCambioMax = 537.5;
    double TipoCambio = generarTipoCambio(TipoCambioMin, TipoCambioMax);

    // Abrir la base de datos
    sqlite3* db = abrirBaseDatos("SistemaBancario.db");

    // Crear las tablas
    crearTablas(db);

    do {
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
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
        switch (option) {
            case 1: {
                int id;
                std::string password;

                cout << "Ingrese el ID de la cuenta: ";
                cin >> id;

                cin.ignore();
                cout << "Ingrese su Password: ";
                getline(cin, password);

                if (!verificarCuenta(db, id, password)) {
                    break; // Volver al menú si la cuenta no se encuentra
                }

                int opcion1;
                
                 do {
                    cout << "Ingrese el servicio que desea recibir: " << endl;
                    cout << "1. Atención al cliente.\n2. Información general sobre Préstamos.\n3. Devolverse" << endl;

                    cout << "Ingrese su selección: ";
                    cin >> opcion1;

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
                
            default:
                cout << "\nOpción inválida...\nPor favor ingrese una opción correcta.\n\n";
                break;
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
