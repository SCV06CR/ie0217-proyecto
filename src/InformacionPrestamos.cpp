#include <regex>
#include "InformacionPrestamos.hpp"

using namespace std;

// Genera el id random con un prefijo (410 colones, 420 dolares)
int generarPrestamoID(int prefijo) {
    /* Inicializacion de la semilla aleatoria: */
    srand(time(NULL));

    return prefijo * 100000 + rand() % 100000;
}

// Sub menu de gestion de prestamos
// Recibe el id de la cuenta
void showMenuSP(const int& idCuenta, sqlite3* db) {

    int opcion;

    do {
        cout << "Ingrese el servicio que desea recibir: " << endl;
        cout << "1. Solicitar un prestano.\n2. Generar reporte de pagos estimados.\n3. Regresar al menú anterior.\n";
        cin >> opcion;

        switch (opcion) {
            case 1:
                gestionarPrestamo(db, idCuenta);
                break;
            case 2:
                reportePrestamos(db);
                break;
            case 3:
                cout << "Regresando..." << endl;
                break;
            default:
                cout << "Opción no válida. Por favor, intente de nuevo." << endl;
                break;
        }
    } while (opcion != 3);
}

// Sub menu de solicitud de prestamos
void gestionarPrestamo(sqlite3* db, const int& idCuenta) {
    int opcionPrestamo;
    int montoColones;
    int montoDolares;

    // si los primeros numeros son 100 es cuenta en colones 
    // Si los primeros numeros son 200 es cuenta en dolares
    // para separar los primeros tres dijitos del id 
    int colones = 100; 
    int dolares = 200; 

    std::string idStr = std::to_string(idCuenta); //convertir a string
    std::string digits = idStr.substr(0, 3); // extraer los primeros 3 caracteres


        // Menú de opciones
        cout << "Seleccione el tipo de prestamo a solicitar:\n"
             << "1. Prestamo Personal\n"
             << "2. Prestamo Prendario\n"
             << "3. Prestamo Hipotecario\n"
             << "4. Regresar al menú anterior.\n";

        int tipoPrestamo;

        cin.ignore(); // Limpiar el buffer de entrada
        cin >> tipoPrestamo;

         if (stoi(digits) == colones) {
            cout << "\nIngrese el monto en colones del Prestamo: ";
            cin >> montoColones;

        } else if(stoi(digits) == dolares) {

            cout << "\nIngrese el monto en dolares del Prestamo: ";
            cin >> montoDolares;
        }

        int interes;
        int plazoPrestamo;

        // Tipos de prestamos disponibles
        string prestamoPersonal = "Prestamo Personal";
        string prestamoPrendario = "Prestamo Prendario";
        string prestamoHipotecario = "Prestamo Hipotecario";

        switch (tipoPrestamo) {
            case 1:
                if (stoi(digits) == colones){
                    cout << "Seleccione una opcion para Prestamo Personal en Colones\n"
                         << "1. Tasa de interes anual del prestamo: 15% y Plazo en meses: 24\n"
                         << "2. Tasa de interes anual del prestamo: 25% y Plazo en meses: 48\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 15;
                        plazoPrestamo = 24;
                    } else if (opcionPrestamo == 2) { 
                        interes = 25;
                        plazoPrestamo = 48;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores correspondientes
                    solicitarPrestamo( db, "Prestamos_Colones", idCuenta, interes, montoColones, plazoPrestamo, prestamoPersonal);
                    
                } else if (stoi(digits) == dolares){
                    cout << "Seleccione una opcion para Prestamo Personal en Dolares\n"
                         << "1. Tasa de interes anual del prestamo: 17% y Plazo en meses: 24\n"
                         << "2. Tasa de interes anual del prestamo: 27% y Plazo en meses: 48\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 17;
                        plazoPrestamo = 24;
                    } else if (opcionPrestamo == 2) { 
                        interes = 27;
                        plazoPrestamo = 48;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Dolares", idCuenta, interes, montoDolares, plazoPrestamo, prestamoPersonal);
                }
                
                break;

            case 2:
                if (stoi(digits) == colones){
                    cout << "Seleccione una opcion para Prestamo Prendario en Colones\n"
                         << "1. Tasa de interes anual del prestamo: 10% y Plazo en meses: 36\n"
                         << "2. Tasa de interes anual del prestamo: 18% y Plazo en meses: 60\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 15;
                        plazoPrestamo = 24;
                    } else if (opcionPrestamo == 2) { 
                        interes = 25;
                        plazoPrestamo = 48;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Colones", idCuenta, interes, montoColones, plazoPrestamo, prestamoPrendario);
                    
                } else if (stoi(digits) == dolares){
                    cout << "Seleccione una opcion para Prestamo Prendario en Dolares\n"
                         << "1. Tasa de interes anual del prestamo: 12% y Plazo en meses: 36\n"
                         << "2. Tasa de interes anual del prestamo: 20% y Plazo en meses: 60\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 12;
                        plazoPrestamo = 36;
                    } else if (opcionPrestamo == 2) { 
                        interes = 20;
                        plazoPrestamo = 60;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Dolares", idCuenta, interes, montoDolares, plazoPrestamo, prestamoPrendario);
                }
                break;

            case 3:
                if (stoi(digits) == colones){
                    cout << "Seleccione una opcion para Prestamo Hipotecario en Colones\n"
                         << "1. Tasa de interes anual del prestamo: 17% y Plazo en meses: 60\n"
                         << "2. Tasa de interes anual del prestamo: 25% y Plazo en meses: 84\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 15;
                        plazoPrestamo = 24;
                    } else if (opcionPrestamo == 2) { 
                        interes = 25;
                        plazoPrestamo = 48;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Colones", idCuenta, interes, montoColones, plazoPrestamo, prestamoHipotecario);
                    
                } else if (stoi(digits) == dolares){
                    cout << "Seleccione una opcion para Prestamo Hipotecario en Dolares\n"
                         << "1. Tasa de interes anual del prestamo: 20% y Plazo en meses: 60\n"
                         << "2. Tasa de interes anual del prestamo: 30% y Plazo en meses: 84\n" ;
                    cin >> opcionPrestamo;
                    
                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 20;
                        plazoPrestamo = 60;
                    } else if (opcionPrestamo == 2) { 
                        interes = 30;
                        plazoPrestamo = 84;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Dolares", idCuenta, interes, montoDolares, plazoPrestamo, prestamoHipotecario);
                }
                break;

            case 4:
                cout << "Regresando..." << endl;
                break;

            default:
                cerr << "Opcion invalida.\n";
                return;
        }

}