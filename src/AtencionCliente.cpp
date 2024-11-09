#include"AtencionCliente.hpp"
#include <regex>
#include<string>
using namespace std; 
/*
************************************************
*Falta hacer la implementacion de las funciones*
************************************************
*/

using namespace std; 

// Esta funcion recibe como argumentos el id de la cuenta a la que se le quiere hacer un deposito
// Recibe como argumento el monto a depositar y recibe la base de datos a la cual se le va a ingresar el dinero. 
void realizadDeposito(const int& id, sqlite3* db){
    //En primer lugar, saber que tipo de cuenta es
    // si los primeros numeros son 100 es cuenta en colones 
    // Si los primeros numeros son 200 es cuenta en dolares
    // para separar los primeros tres dijitos del id 
    int colones = 100; 
    int dolares = 200; 
    double monto; 
    std::string idStr = std::to_string(id); //convertir a string
    std::string digits = idStr.substr(0, 3); // extraer los primeros 3 caracteres
    // Definiendo consulta SQL segun el tipo de cuenta
    std::string query; 

    if (std::stoi(digits) == colones){
        query = "UPDATE Cuenta_Clones SET cantidad_dinero = cantidad_dinero + ? WHERE id = ?"; 
        cout << "Ingrese el monto en colones que desea depositar en la cuenta: \n"; 
        cin >> monto;
    } if (std::stoi(digits) == dolares){
        cout << "Ingrese el monto en colones que desea depositar en la cuenta: \n"; 
        cin >> monto;
        query = "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero + ? WHERE id = ?;";
    };

    // Preparar consulta de sql 
    sqlite3_stmt* stmt; 
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl; 
        return;
    }

    // Asignar valores a los parametros
    sqlite3_bind_double(stmt, 1, monto); //Vinculat el monto a ? en la consulta
    sqlite3_bind_text(stmt, 2, idStr.c_str(), -1, SQLITE_STATIC); // Vincular el id a ? en la consulta

    // Ejecutar la consulta
    if(sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << endl; 
    } else {
        cout << "Deposito de " << monto << "a la cuenta: " << id << ", realizado con éxito." <<endl; 
    }

    // Falta agregar el movimiento a la tabla de estados de cuenta.

    // Limpiar el statement
    sqlite3_finalize(stmt); 

}

// Esta funcion recibe como argumentos el id de la cuenta a la que se le quiere hacer un deposito
// Recibe como argumento el monto a depositar
// Recibe la base de datos donde existe la cuenta a la cual se le va a ingresar el dinero.
void realizadRetiro(const int& id, const double& monto, sqlite3* db){
    
}


// El primer const id es para la cuenta de origen 
// El segundo const id es para la cuenta destino 
// se recibe el monto a transferir 
// Recibe la base de datos donde existe la cuenta a la cual se le va a ingresar el dinero.
void realizadTransferencia(const int& idOrigen, const string& idDestino, sqlite3* db){

}

// El primer const id es del préstamo
// El segundo const id es de la cuenta
// se recibe el monto para abonar
// Recibe la base de datos donde existe la cuenta y el préstamo.
void realizarAbono(const int& idPrestamo, const string& idCuenta, const double& monto, sqlite3* db){
    // de momento esta funion queda asi porque no se ha implementado la parte de prestamos
}


// El primer const id es del préstamo
// El segundo const id es de la cuenta
// Recibe la base de datos donde existe la cuenta y el préstamo.
void pagarCuota(const int& idPrestamo, const string& idCuenta, sqlite3* db){
    // de momento esta funion queda asi porque no se ha implementado la parte de prestamos 
}

// Recibe el id de la cuenta la cual se quiere registrar una salida del pais 
// Recibe la base de datos donde existe la cuenta
void registrarSalidaPais(const int& idCuenta, sqlite3* db){
    
}

// Recibe el id de la cuenta la cual se quiere consultar PIN del cajero 
// Recibe la base de datos donde existe la cuenta
void consultarPin(const int& idCuenta, sqlite3* db){

}


// Recibe el id de la cuenta la cual se quiere consultar el CVV
// Recibe la base de datos donde existe la cuenta
void consultarCVV(const int& idCuenta, sqlite3* db){

}


// Recibe el id de la cuenta que se quiere realizar la impresión del estado de cuenta
// Recibe una base de datos 
void imprimirEstadoCuenta(const int& idCuenta, sqlite3* db){

}

// Funcion para desplegar el menu de opciones de atencion al cliente
string showMenuAC(const int& id, sqlite3* db){
    int opcion;
    do {
        cout << "Ingrese el servicio que desea recibir: " << endl;
        cout << "1. Realizar depósito.\n2. Realizar retiro.\n3. Realizar transferencia.\n4.Realizar abono.\n5. Pagar cuota.\n6. Registrar salida del pais.\n7. Consultar pin.\n 8. Consultar CVV.\n 9. Imprimir estado de cuenta.\n 10. Regresar al menu anterior.\n";
        cin >> opcion;

        switch (opcion) {
            case 1:
                realizadDeposito(id, db); 
                break;
            case 2:
                cout << "\nAquí se desplegará el menú de info de préstamos\n" << endl;
                 break;           
            case 3:
                cout << "\nAquí se devuelve al menú anterior" << endl;
                break;
            case 4:
                cout << "\nAquí se devuelve al menú anterior" << endl;
                break;  

            case 5:
                cout << "\nAquí se devuelve al menú anterior" << endl;
                break; 
            case 6:
                cout << "\nAquí se devuelve al menú anterior" << endl;
                break; 
            case 7:
                cout << "\nAquí se devuelve al menú anterior" << endl;
                break;
            case 8:
                cout << "\nAquí se devuelve al menú anterior" << endl;
                break;
            case 9:
                cout << "\nAquí se devuelve al menú anterior" << endl;
                break;
            case 10:
                cout << "\nSaliendo del programa..." << endl;
                break;
            default:
                cout << "\nOpción inválida...\nPor favor intente de nuevo.\n" << endl;
        }
    } while (opcion != 10);
    return 0; 
}

