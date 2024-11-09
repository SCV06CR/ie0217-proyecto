#include <regex>
#include "InformacionPrestamos.hpp"

// Genera el id random con un prefijo (100 colones, 200 dolares)
int generarPrestamoID(int prefijo) {
    return prefijo * 100000 + rand() % 100000;
}

void gestionarPrestamo(sqlite3* db, const int& idCuenta) {
    int opcion;
    int interes;
    int montoColones;
    int montoDolares;
    int plazoPrestamo;
    string prestamoPersonal = "Prestamo Personal";
    string prestamoPrendario = "Prestamo Prendario";
    string prestamoHipotecario = "Prestamo Hipotecario";

    // si los primeros numeros son 100 es cuenta en colones 
    // Si los primeros numeros son 200 es cuenta en dolares
    // para separar los primeros tres dijitos del id 
    int colones = 100; 
    int dolares = 200; 

    std::string idStr = std::to_string(idCuenta); //convertir a string
    std::string digits = idStr.substr(0, 3); // extraer los primeros 3 caracteres


    // Menú de opciones
    cout << "Seleccione el tipo de prestamo a solicitar:\n";
    cout << "1. Prestamo Personal\n";
    cout << "2. Prestamo Prendario\n";
    cout << "3. Prestamo Hipotecario\n";
    cin >> opcion;
    cin.ignore(); // Limpiar el buffer de entrada

    switch (opcion) {
        case 1:

            if (std::stoi(digits) == colones){
                cout << "Prestamo Personal en Colones\n";

                // Se debe crear una funcion para delimitar minimos y maximos
                cout << "Ingrese el monto en colones que desea depositar en la cuenta: \n"; 
                cout << "Minimo permitido 100 000 de colones\n";
                cout << "Maximo permitido 10 000 000 de colones\n";
                cin >> montoColones; 

                cout << "Digite la tasa de interes mensual del prestamo:\n";
                cout << "Minimo permitido 10'%'\n";
                cout << "Maximo permitido 40'%'\n";
                cin >> interes;

                cout << "Seleccione el plazo en meses del prestamo:\n";
                cout << "Minimo permitido 12 meses \n";
                cout << "Maximo permitido 48 meses \n";
                cin >> plazoPrestamo;

                // Registra en la base de datos los valores ingresados
                solicitarPrestamo( db, "Prestamos_Colones", idCuenta, interes, montoColones, plazoPrestamo, prestamoPersonal);
            } else if (std::stoi(digits) == dolares){
                cout << "Prestamo Personal en Colones\n";

                // Se debe crear una funcion para delimitar minimos y maximos
                cout << "Ingrese el monto en Dolares que desea depositar en la cuenta: \n"; 
                cout << "Minimo permitido 500 Dolares\n";
                cout << "Maximo permitido 9000 Dolares\n";
                cin >> montoDolares;

                cout << "Digite la tasa de interes mensual del prestamo:\n";
                cout << "Minimo permitido 15'%'\n";
                cout << "Maximo permitido 45'%'\n";
                cin >> interes;

                cout << "Seleccione el plazo en meses del prestamo:\n";
                cout << "Minimo permitido 12 meses \n";
                cout << "Maximo permitido 48 meses \n";
                cin >> plazoPrestamo;

                // Registra en la base de datos los valores ingresados
                solicitarPrestamo( db, "Prestamos_Dolares", idCuenta, interes, montoDolares, plazoPrestamo, prestamoPersonal);
            }
            
            break;

        case 2:
            break;

        case 3:
            cout << "Regresando..." << endl;
            break;

        default:
            cerr << "Opcion invalida.\n";
            return;
    }

}

void solicitarPrestamo(sqlite3* db, const string& tabla,const int& idCuenta, int& interes, int& monto, int& plazoPrestamo, const string& tipoPrestamo) {


    int idPrestamo = generarPrestamoID(4);

    string sql = "INSERT INTO " + tabla + " (id_prestamo, id_cuenta, intereses, meses, monto, tipo_prestamo, cuotas_pagadas) VALUES (" + to_string(idPrestamo) + ", " + to_string(idCuenta) + ", " + to_string(interes) + ", " + to_string(plazoPrestamo) + ", " + to_string(monto) + ",'" + tipoPrestamo + "', 0);";

    ejecutarSQL(db, sql.c_str());

}