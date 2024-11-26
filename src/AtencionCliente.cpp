#include "AtencionCliente.hpp"
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
    string idStr = to_string(id); //convertir a string
    string digits = idStr.substr(0, 3); // extraer los primeros 3 caracteres
    // Definiendo consulta SQL segun el tipo de cuenta
    string query; 

    if (stoi(digits) == colones){
        query = "UPDATE Cuenta_Colones SET cantidad_dinero = cantidad_dinero + ? WHERE id = ?"; 
        cout << "Ingrese el monto en colones que desea depositar en la cuenta: \n"; 
        cin >> monto;
    } if (stoi(digits) == dolares){
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
void realizadRetiro(const int& id, sqlite3* db){
    int colones = 100; 
    int dolares = 200; 
    double retiro; 
    string idStr = to_string(id); //convertir a string
    string digits = idStr.substr(0, 3); // extraer los primeros 3 caracteres
    
    cout << "Ingrese la cantidad de dinero que desea retirar" << endl;
    cin >> retiro; 
    // Definiendo consulta SQL segun el tipo de cuenta
    string query; 

    if (stoi(digits) == colones){
        cout << "Ingrese la cantidad de dinero que desea retirar" << endl;
        cin >> retiro; 

        //Quiero comprobar que la cuenta no quede con dinero negativo
        string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Dolares WHERE id = ?";
        sqlite3_stmt* checkStmt;
        sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
        sqlite3_bind_text(checkStmt, 1, idStr.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            double currentBalance = sqlite3_column_double(checkStmt, 0);
            if (currentBalance - retiro < 0) {
                cout << " No se puede retirar esa cantidad de dinero. Fondos insuficientes." << endl;
                sqlite3_finalize(checkStmt);
                return;
            } //Agregar un else con el agregado la actualizacion de la base de datos
        }
        sqlite3_finalize(checkStmt);


        query = "UPDATE Cuenta_Clones SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?"; 
    } if (stoi(digits) == dolares){

        cout << "Ingrese la cantidad de dinero que desea retirar" << endl;
        cin >> retiro; 

        // Definiendo consulta SQL segun el tipo de cuenta
        string query; 

        if (stoi(digits) == colones){
            cout << "Ingrese la cantidad de dinero que desea retirar" << endl;
            cin >> retiro; 

            //Quiero comprobar que la cuenta no quede con dinero negativo
            string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Colones WHERE id = ?";
            sqlite3_stmt* checkStmt;
            sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
            sqlite3_bind_text(checkStmt, 1, idStr.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(checkStmt) == SQLITE_ROW) {
                double currentBalance = sqlite3_column_double(checkStmt, 0);
                if (currentBalance - retiro < 0) {
                    cout << " No se puede retirar esa cantidad de dinero. Fondos insuficientes." << endl;
                    sqlite3_finalize(checkStmt);
                    return;
                }
            }
            sqlite3_finalize(checkStmt);

        query = "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero + ? WHERE id = ?;";
        }

        // Preparar consulta de sql 
        sqlite3_stmt* stmt; 
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
            cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl; 
            return;
        }

        // Asignar valores a los parametros
        sqlite3_bind_double(stmt, 1, retiro); //Vinculat el retiro a ? en la consulta
        sqlite3_bind_text(stmt, 2, idStr.c_str(), -1, SQLITE_STATIC); // Vincular el id a ? en la consulta

        // Ejecutar la consulta
        if(sqlite3_step(stmt) != SQLITE_DONE){
            cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << endl; 
        } else {
            cout << "Retiro de " << retiro << "a la cuenta: " << id << ", realizado con éxito." <<endl; 
        }

        // Falta agregar el movimiento a la tabla de estados de cuenta.

        // Limpiar el statement
        sqlite3_finalize(stmt); 
    }
}



// El primer const id es para la cuenta de origen 
// El segundo const id es para la cuenta destino 
// se recibe el monto a transferir 
// Recibe la base de datos donde existe la cuenta a la cual se le va a ingresar el dinero.
void realizadTransferencia(const int& idOrigen, const int& idDestino, const double& TipoCambio, sqlite3* db) {
    int colones = 100; 
    int dolares = 200; 
    double transfer; 
    string idOrig = std::to_string(idOrigen); // Convertir a string
    string idDest = std::to_string(idDestino); //convertir a string
    string digOrig = idOrig.substr(0, 3); // extraer los primeros 3 caracteres
    string digDest = idDest.substr(0, 3); // Extraer los primeros 3 caracteres

    cout << "Ingrese la cantidad de dinero que desea transferir." << std::endl;
    cin >> transfer; 

    // Definiendo consulta SQL según el tipo de cuenta
    string query;

    if (std::stoi(digOrig) == colones && std::stoi(digDest) == colones) {
        // Comprobar que la cuenta de origen no quede en negativo
        string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Clones WHERE id = ?";
        sqlite3_stmt* checkStmt;
        sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
        sqlite3_bind_text(checkStmt, 1, idOrig.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            double currentBalance = sqlite3_column_double(checkStmt, 0);
            if (currentBalance - transfer < 0) {
                cout << "No se puede retirar esa cantidad de dinero. Fondos insuficientes." << endl;
                sqlite3_finalize(checkStmt);
                return;
            }
        }
        sqlite3_finalize(checkStmt);

        query = "UPDATE Cuenta_Clones SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?; "
                "UPDATE Cuenta_Clones SET cantidad_dinero = cantidad_dinero + ? WHERE id = ?; ";

    } else if (std::stoi(digOrig) == dolares && std::stoi(digDest) == dolares) {
        // Comprobar que la cuenta de origen no quede en negativo
        std::string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Dolares WHERE id = ?";
        sqlite3_stmt* checkStmt;
        sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
        sqlite3_bind_text(checkStmt, 1, idOrig.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            double currentBalance = sqlite3_column_double(checkStmt, 0);
            if (currentBalance - transfer < 0) {
                cout << "No se puede retirar esa cantidad de dinero. Fondos insuficientes." << endl;
                sqlite3_finalize(checkStmt);
                return;
            }
        }
        sqlite3_finalize(checkStmt);

        query ="UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?; "
                "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero + ? WHERE id = ?; ";

    } else if (std::stoi(digOrig) == colones && std::stoi(digDest) == dolares) {
        // Comprobar que la cuenta de origen no quede en negativo
        std::string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Clones WHERE id = ?";
        sqlite3_stmt* checkStmt;
        sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
        sqlite3_bind_text(checkStmt, 1, idOrig.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            double currentBalance = sqlite3_column_double(checkStmt, 0);
            if (currentBalance - transfer < 0) {
                std::cout << "No se puede retirar esa cantidad de dinero. Fondos insuficientes." << std::endl;
                sqlite3_finalize(checkStmt);
                return;
            }
        }
        sqlite3_finalize(checkStmt);

        // Realizando la conversion con el tipo de cambio de colones a dolares
        double transferDolares = transfer * TipoCambio;

        query ="UPDATE Cuenta_Clones SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?; "
                "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero + ? WHERE id = ?; ";

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
        sqlite3_bind_double(stmt, 1, transfer);
        sqlite3_bind_text(stmt, 2, idOrig.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, transferDolares);
        sqlite3_bind_text(stmt, 4, idDest.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Transferencia de " << transfer << " desde la cuenta " << idOrigen << " a la cuenta " << idDestino << " realizada con éxito." << std::endl;
        }
        sqlite3_finalize(stmt);

        return;

    } else if (std::stoi(digOrig) == dolares && std::stoi(digDest) == colones) {
        // Comprobar que la cuenta de origen no quede en negativo
        std::string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Dolares WHERE id = ?";
        sqlite3_stmt* checkStmt;
        sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
        sqlite3_bind_text(checkStmt, 1, idOrig.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            double currentBalance = sqlite3_column_double(checkStmt, 0);
            if (currentBalance - transfer < 0) {
                std::cout << "No se puede retirar esa cantidad de dinero. Fondos insuficientes." << std::endl;
                sqlite3_finalize(checkStmt);
                return;
            }
        }
        sqlite3_finalize(checkStmt);

        // Convertir la cantidad de dólares a colones antes de la transferencia
        double transferColones = transfer / TipoCambio;

        query = "BEGIN TRANSACTION; "
                "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?; "
                "UPDATE Cuenta_Clones SET cantidad_dinero = cantidad_dinero + ? WHERE id = ?; "
                "COMMIT;";

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
        sqlite3_bind_double(stmt, 1, transfer);
        sqlite3_bind_text(stmt, 2, idOrig.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, transferColones);
        sqlite3_bind_text(stmt, 4, idDest.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Transferencia de " << transfer << " desde la cuenta " << idOrigen << " a la cuenta " << idDestino << " realizada con éxito." << std::endl;
        }
        sqlite3_finalize(stmt);

        return;
    }
}


// El primer const id es del préstamo
// El segundo const id es de la cuenta
// se recibe el monto para abonar
// Recibe la base de datos donde existe la cuenta y el préstamo.
void realizarAbono(const int& idPrestamo, const double& tipoCambio, const int& idCuenta, sqlite3* db) {
    // Determinar el tipo de cuenta según el prefijo del ID
    string idCuentaStr = to_string(idCuenta);
    string cuentaPrefix = idCuentaStr.substr(0, 3);

    int cuentaEsColones = (cuentaPrefix == "100");
    int cuentaEsDolares = (cuentaPrefix == "200");

    // Consultar el tipo de préstamo (colones o dólares)
    string queryPrestamo;
    if (cuentaEsColones) {
        queryPrestamo = "SELECT monto, cuotas_pagadas FROM Prestamos_Colones WHERE id_prestamo = ?";
    } else if (cuentaEsDolares) {
        queryPrestamo = "SELECT monto, cuotas_pagadas FROM Prestamos_Dolares WHERE id_prestamo = ?";
    } else {
        cerr << "El ID de la cuenta no corresponde a un tipo válido." << endl;
        return;
    }

    double montoRestante;
    int cuotasPagadas;

    sqlite3_stmt* stmtPrestamo;
    if (sqlite3_prepare_v2(db, queryPrestamo.c_str(), -1, &stmtPrestamo, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta del préstamo: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_int(stmtPrestamo, 1, idPrestamo);

    if (sqlite3_step(stmtPrestamo) == SQLITE_ROW) {
        montoRestante = sqlite3_column_double(stmtPrestamo, 0);
        cuotasPagadas = sqlite3_column_int(stmtPrestamo, 1);
    } else {
        cerr << "No se encontró el préstamo con ID " << idPrestamo << endl;
        sqlite3_finalize(stmtPrestamo);
        return;
    }
    sqlite3_finalize(stmtPrestamo);

    // Consultar el saldo de la cuenta
    string querySaldoCuenta = cuentaEsColones
        ? "SELECT cantidad_dinero FROM Cuenta_Colones WHERE id = ?"
        : "SELECT cantidad_dinero FROM Cuenta_Dolares WHERE id = ?";
    double saldoCuenta;

    sqlite3_stmt* stmtCuenta;
    if (sqlite3_prepare_v2(db, querySaldoCuenta.c_str(), -1, &stmtCuenta, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta de saldo de cuenta: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_int(stmtCuenta, 1, idCuenta);

    if (sqlite3_step(stmtCuenta) == SQLITE_ROW) {
        saldoCuenta = sqlite3_column_double(stmtCuenta, 0);
    } else {
        cerr << "No se encontró la cuenta con ID " << idCuenta << endl;
        sqlite3_finalize(stmtCuenta);
        return;
    }
    sqlite3_finalize(stmtCuenta);

    // Solicitar el monto a abonar
    double montoAbono;
    cout << "Ingrese el monto que desea abonar: ";
    cin >> montoAbono;

    // Validar y realizar el abono
    if (cuentaEsColones && montoRestante > 0) {
        // La cuenta y el préstamo están en colones
        if (saldoCuenta - montoAbono < 1) {
            cerr << "No se puede realizar el abono. La cuenta no puede quedar con saldo menor que 1." << endl;
            return;
        }
    } else if (cuentaEsDolares && montoRestante > 0) {
        // La cuenta y el préstamo están en dólares
        if (saldoCuenta - montoAbono < 1) {
            cerr << "No se puede realizar el abono. La cuenta no puede quedar con saldo menor que 1." << endl;
            return;
        }
    } else if (cuentaEsDolares && montoRestante > 0) {
        // Conversión de dólares a colones
        double montoConvertido = montoAbono * (tipoCambio);
        if (saldoCuenta - montoConvertido < 1) {
            cerr << "No se puede realizar el abono. La cuenta no puede quedar con saldo menor que 1 después de la conversión." << endl;
            return;
        }
        montoAbono = montoConvertido;
    } else if (cuentaEsColones && montoRestante > 0) {
        // Conversión de colones a dólares
        double montoConvertido = montoAbono / (tipoCambio);
        if (saldoCuenta - montoConvertido < 1) {
            cerr << "No se puede realizar el abono. La cuenta no puede quedar con saldo menor que 1 después de la conversión." << endl;
            return;
        }
        montoAbono = montoConvertido;
    }

    // Actualizar saldo de la cuenta
    string queryUpdateCuenta = cuentaEsColones
        ? "UPDATE Cuenta_Colones SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?"
        : "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?";
    sqlite3_stmt* stmtUpdateCuenta;
    if (sqlite3_prepare_v2(db, queryUpdateCuenta.c_str(), -1, &stmtUpdateCuenta, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta de actualización de cuenta: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_double(stmtUpdateCuenta, 1, montoAbono);
    sqlite3_bind_int(stmtUpdateCuenta, 2, idCuenta);

    if (sqlite3_step(stmtUpdateCuenta) != SQLITE_DONE) {
        cerr << "Error al ejecutar la actualización de la cuenta: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmtUpdateCuenta);
        return;
    }
    sqlite3_finalize(stmtUpdateCuenta);

    // Actualizar monto restante del préstamo
    string queryUpdatePrestamo = cuentaEsColones
        ? "UPDATE Prestamos_Colones SET monto = monto - ? WHERE id_prestamo = ?"
        : "UPDATE Prestamos_Dolares SET monto = monto - ? WHERE id_prestamo = ?";
    sqlite3_stmt* stmtUpdatePrestamo;
    if (sqlite3_prepare_v2(db, queryUpdatePrestamo.c_str(), -1, &stmtUpdatePrestamo, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta de actualización del préstamo: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_double(stmtUpdatePrestamo, 1, montoAbono);
    sqlite3_bind_int(stmtUpdatePrestamo, 2, idPrestamo);

    if (sqlite3_step(stmtUpdatePrestamo) != SQLITE_DONE) {
        cerr << "Error al ejecutar la actualización del préstamo: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmtUpdatePrestamo);
        return;
    }
    sqlite3_finalize(stmtUpdatePrestamo);

    cout << "Abono realizado con éxito. Monto descontado: " << montoAbono << ". Préstamo actualizado correctamente." << endl;
}

// El primer const id es del préstamo
// El segundo const id es de la cuenta
// Recibe la base de datos donde existe la cuenta y el préstamo.
void pagarCuota(const int& idPrestamo, const int& idCuenta, sqlite3* db){
    cout << "Todavía falta de implementar" << endl;
    // de momento esta funion queda asi porque no se ha implementado la parte de prestamos 
}

// Recibe el id de la cuenta la cual se quiere registrar una salida del pais 
// Recibe la base de datos donde existe la cuenta
void registrarSalidaPais(const int& idCuenta, sqlite3* db) {
    // En primer lugar, saber qué tipo de cuenta es
    
    int colones = 100; // Si los primeros números son 100, es cuenta en colones
    int dolares = 200; // Si los primeros números son 200, es cuenta en dólares
    string idStr = to_string(idCuenta); // Convertir a string
    string digits = idStr.substr(0, 3); // Extraer los primeros 3 caracteres
    
    // Definiendo consulta SQL según el tipo de cuenta
    string query; 

    if (std::stoi(digits) == colones) {
        query = "UPDATE Cuenta_Colones SET salida_pais = 1 WHERE id = ?";
    } else if (stoi(digits) == dolares) {
        query = "UPDATE Cuenta_Dolares SET salida_pais = 1 WHERE id = ?";
    }

    // Preparar consulta de SQL
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl; 
        return;
    }

    // Asignar valores a los parámetros
    sqlite3_bind_int(stmt, 1, idCuenta); // Vincular el id a ? en la consulta

    // Ejecutar la consulta
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << endl; 
    } else {
        cout << "Se ha registrado la tarjeta con permiso de salida del país para el id: " << idCuenta << endl; 
    }

    // Limpiar el statement
    sqlite3_finalize(stmt); 
}


// Recibe el id de la cuenta la cual se quiere consultar PIN del cajero 
// Recibe la base de datos donde existe la cuenta
void consultarPin(const int& idCuenta, sqlite3* db) {
    // En primer lugar, saber qué tipo de cuenta es
    // Si los primeros números son 100, es cuenta en colones
    // Si los primeros números son 200, es cuenta en dólares
    int colones = 100; 
    int dolares = 200; 
    std::string idStr = std::to_string(idCuenta); // Convertir a string
    std::string digits = idStr.substr(0, 3); // Extraer los primeros 3 caracteres

    // Definiendo consulta SQL según el tipo de cuenta
    std::string query; 

    if (std::stoi(digits) == colones) {
        query = "SELECT pin FROM Cuenta_Colones WHERE id = ?";
    } else if (std::stoi(digits) == dolares) {
        query = "SELECT pin FROM Cuenta_Dolares WHERE id = ?";
    }

    // Preparar la consulta de selección
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Vincular el id de la cuenta
    sqlite3_bind_int(stmt, 1, idCuenta);

    // Ejecutar la consulta y recuperar el resultado
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int pin = sqlite3_column_int(stmt, 0);
        std::cout << "El PIN de la cuenta con ID " << idCuenta << " es: " << pin << std::endl;
    } else {
        std::cerr << "No se encontró ninguna cuenta con el ID proporcionado." << std::endl;
    }

    // Limpiar el statement
    sqlite3_finalize(stmt);
}




// Recibe el id de la cuenta la cual se quiere consultar el CVV
// Recibe la base de datos donde existe la cuenta
void consultarCVV(const int& idCuenta, sqlite3* db) {
    // En primer lugar, saber qué tipo de cuenta es
    // Si los primeros números son 100, es cuenta en colones
    // Si los primeros números son 200, es cuenta en dólares
    int colones = 100; 
    int dolares = 200; 
    std::string idStr = std::to_string(idCuenta); // Convertir a string
    std::string digits = idStr.substr(0, 3); // Extraer los primeros 3 caracteres

    // Definiendo consulta SQL según el tipo de cuenta
    std::string query; 

    if (std::stoi(digits) == colones) {
        query = "SELECT cvv FROM Cuenta_Colones WHERE id = ?";
    } else if (std::stoi(digits) == dolares) {
        query = "SELECT cvv FROM Cuenta_Dolares WHERE id = ?";
    }

    // Preparar la consulta de selección
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Vincular el id de la cuenta
    sqlite3_bind_int(stmt, 1, idCuenta);

    // Ejecutar la consulta y recuperar el resultado
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int cvv = sqlite3_column_int(stmt, 0);
        std::cout << "El CVV (PIN) de la cuenta con ID " << idCuenta << " es: " << cvv << std::endl;
    } else {
        std::cerr << "No se encontró ninguna cuenta con el ID proporcionado." << std::endl;
    }

    // Limpiar el statement
    sqlite3_finalize(stmt);
}


// Recibe el id de la cuenta que se quiere realizar la impresión del estado de cuenta
// Recibe una base de datos 
void imprimirEstadoCuenta(const int& idCuenta, sqlite3* db){
    cout << "Todavía falta de implementar" << endl;
}

// Funcion para desplegar el menu de opciones de atencion al cliente
string showMenuAC(const int& idCuenta, const double& TipoCambio, sqlite3* db) {
    int opcion;
    int idPrestamo;
    int idDest;

    do {
        cout << "Ingrese el servicio que desea recibir: " << endl;
        cout << "1. Realizar depósito.\n2. Realizar retiro.\n3. Realizar transferencia.\n4. Realizar abono.\n5. Pagar cuota.\n6. Registrar salida del país.\n7. Consultar PIN.\n8. Consultar CVV.\n9. Imprimir estado de cuenta.\n10. Regresar al menú anterior.\n";
        cin >> opcion;

        switch (opcion) {
            case 1:
                realizadDeposito(idCuenta, db);
                break;
            case 2:
                realizadRetiro(idCuenta, db);
                break;
            case 3:
                cout << "Ingrese el número de cuenta al que desea realizar la transferencia." << endl;
                cin >> idDest;
                realizadTransferencia(idCuenta, idDest, TipoCambio, db);
                break;
            case 4:
                cout << "Ingrese el número de depósito" << endl;
                cin >> idPrestamo;
                realizarAbono(idPrestamo, TipoCambio, idCuenta,  db);
                break;
            case 5:
                cout << "Ingrese el número de depósito" << endl;
                cin >> idPrestamo;
                pagarCuota(idPrestamo, idCuenta, db);
                break;
            case 6:
                registrarSalidaPais(idCuenta, db);
                break;
            case 7:
                consultarPin(idCuenta, db);
                break;
            case 8:
                consultarCVV(idCuenta, db);
                break;
            case 9:
                imprimirEstadoCuenta(idCuenta, db);
                break;
            case 10:
                cout << "Regresando..." << endl;
                break;
            default:
                cout << "Opción no válida. Por favor, intente de nuevo." << endl;
                break;
        }
    } while (opcion != 10);
    return "";
}
