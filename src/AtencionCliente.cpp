#include "AtencionCliente.hpp"
#include <regex>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std; 


/**
 * @brief Realiza un depósito en una cuenta bancaria.
 * 
 * Esta función permite depositar dinero en una cuenta bancaria. Determina el tipo de cuenta
 * (colones o dólares) a partir de los primeros tres dígitos del ID de la cuenta, actualiza
 * el saldo de la cuenta correspondiente en la base de datos y registra el movimiento en
 * el historial de transacciones.
 * 
 * @param id El ID de la cuenta a la que se realizará el depósito.
 *           Los primeros tres dígitos determinan el tipo de cuenta:
 *           - 100: Cuenta en colones.
 *           - 200: Cuenta en dólares.
 * @param db Un puntero a la base de datos SQLite donde se actualizará el saldo de la cuenta.
 * 
 * @details
 * - La función solicita al usuario el monto a depositar.
 * - Actualiza el saldo de la cuenta en la tabla correspondiente (`Cuenta_Colones` o `Cuenta_Dolares`).
 * - Registra el movimiento del depósito en la tabla de movimientos utilizando la función `AgregarMov`.
 * 
 * @throws Error en caso de problemas al preparar o ejecutar la consulta SQL.
 * 
 */ 
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

        ostringstream detalle;
        detalle << "Depósito de " << monto ;
        // Convertir a detalle a string
        const string detalleTexto = detalle.str();

        cout << detalleTexto << "a la cuenta: " << id << ", realizado con éxito." <<endl; 
        
        AgregarMov(id, detalleTexto, db); 

    }

    // Falta agregar el movimiento a la tabla de estados de cuenta.

    // Limpiar el statement
    sqlite3_finalize(stmt); 

}

/**
 * @brief Realiza un retiro de una cuenta bancaria.
 * 
 * Esta función permite retirar dinero de una cuenta bancaria. Determina el tipo de cuenta
 * (colones o dólares) a partir de los primeros tres dígitos del ID de la cuenta, verifica 
 * si hay suficiente saldo disponible y actualiza el saldo de la cuenta correspondiente 
 * en la base de datos. Además, registra el movimiento en el historial de transacciones.
 * 
 * @param id El ID de la cuenta de la cual se realizará el retiro.
 *           Los primeros tres dígitos determinan el tipo de cuenta:
 *           - 100: Cuenta en colones.
 *           - 200: Cuenta en dólares.
 * @param db Un puntero a la base de datos SQLite donde se actualizará el saldo de la cuenta.
 * 
 * @details
 * - La función solicita al usuario el monto que desea retirar.
 * - Verifica que haya fondos suficientes en la cuenta antes de realizar el retiro.
 * - Si hay fondos suficientes, actualiza el saldo de la cuenta en la tabla correspondiente
 *   (`Cuenta_Colones` o `Cuenta_Dolares`) y registra el movimiento del retiro en la tabla
 *   de movimientos utilizando la función `AgregarMov`.
 * - Si no hay fondos suficientes, informa al usuario y registra un movimiento indicando 
 *   el intento fallido.
 * 
 * @throws Error en caso de problemas al preparar o ejecutar la consulta SQL.
 * 
 */
void realizadRetiro(const int& id, sqlite3* db){
    int colones = 100; 
    int dolares = 200; 
    double retiro; 
    string idStr = to_string(id); // Convertir a string
    string digits = idStr.substr(0, 3); // Extraer los primeros 3 caracteres
    
    cout << "Ingrese la cantidad de dinero que desea retirar: " << endl;
    cin >> retiro; 

    // Definiendo consulta SQL según el tipo de cuenta
    string query; 

    if (stoi(digits) == colones){
        // Verificar que la cuenta tenga suficiente saldo
        string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Colones WHERE id = ?";
        sqlite3_stmt* checkStmt;
        sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
        sqlite3_bind_text(checkStmt, 1, idStr.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            double currentBalance = sqlite3_column_double(checkStmt, 0);
            if (currentBalance - retiro < 0) {
                ostringstream detalle;
                detalle << "Retiro de: " << retiro << " colones, no realizado por fondos insuficientes.";
                const string detalleTexto = detalle.str();
                AgregarMov(id, detalleTexto, db);  // Registrar movimiento

                cout << "No se puede retirar esa cantidad de dinero. Fondos insuficientes." << endl;
                sqlite3_finalize(checkStmt);
                return;
            } else {
                // Actualizar el saldo de la cuenta
                query = "UPDATE Cuenta_Colones SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?";
                sqlite3_stmt* stmt;
                if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
                    cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
                    sqlite3_finalize(checkStmt);
                    return;
                }

                // Asignar valores a los parámetros
                sqlite3_bind_double(stmt, 1, retiro); // Vincular el retiro
                sqlite3_bind_text(stmt, 2, idStr.c_str(), -1, SQLITE_STATIC); // Vincular el ID

                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << endl;
                } else {
                    // Agregar el movimiento a la tabla de movimientos
                    ostringstream detalle;
                    detalle << "Retiro de: " << retiro << " colones, realizado con éxito." << endl;
                    const string detalleTexto = detalle.str();
                    AgregarMov(id, detalleTexto, db);  // Registrar movimiento

                    cout << "Retiro de " << retiro << " colones de la cuenta: " << id << ", realizado con éxito." << endl;
                }

                sqlite3_finalize(stmt);
            }
        }
        sqlite3_finalize(checkStmt);
    }

    else if (stoi(digits) == dolares){
        // Verificar que la cuenta tenga suficiente saldo
        string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Dolares WHERE id = ?";
        sqlite3_stmt* checkStmt;
        sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
        sqlite3_bind_text(checkStmt, 1, idStr.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            double currentBalance = sqlite3_column_double(checkStmt, 0);
            if (currentBalance - retiro < 0) {
                ostringstream detalle;
                detalle << "Retiro de: " << retiro << " dolares, no realizado por fondos insuficientes.";
                const string detalleTexto = detalle.str();
                AgregarMov(id, detalleTexto, db);  // Registrar movimiento

                cout << "No se puede retirar esa cantidad de dinero. Fondos insuficientes." << endl;
                sqlite3_finalize(checkStmt);
                return;
            } else {
                // Actualizar el saldo de la cuenta
                query = "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?";
                sqlite3_stmt* stmt;
                if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
                    cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
                    sqlite3_finalize(checkStmt);
                    return;
                }

                // Asignar valores a los parámetros
                sqlite3_bind_double(stmt, 1, retiro); // Vincular el retiro
                sqlite3_bind_text(stmt, 2, idStr.c_str(), -1, SQLITE_STATIC); // Vincular el ID

                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << endl;
                } else {
                    // Agregar el movimiento a la tabla de movimientos
                    ostringstream detalle;
                    detalle << "Retiro de: " << retiro << " dolares, realizado con éxito."<< endl;
                    const string detalleTexto = detalle.str();
                    AgregarMov(id, detalleTexto, db);  // Registrar movimiento

                    cout << "Retiro de " << retiro << " dolares de la cuenta: " << id << ", realizado con éxito." << endl;
                }

                sqlite3_finalize(stmt);
            }
        }
        sqlite3_finalize(checkStmt);
    }
    else {
        cout << "ID de cuenta no válido." << endl;
    }
}

/**
 * @brief Realiza una transferencia de dinero entre dos cuentas bancarias.
 * 
 * Esta función permite transferir dinero desde una cuenta de origen a una cuenta de destino.
 * Verifica que la cuenta de origen tenga saldo suficiente, maneja conversiones entre colones 
 * y dólares según el tipo de cambio proporcionado, y actualiza los saldos en la base de datos.
 * Además, registra los movimientos de la operación en la tabla de movimientos.
 * 
 * @param idOrigen ID de la cuenta de origen de la transferencia.
 * @param idDestino ID de la cuenta de destino de la transferencia.
 * @param TipoCambio Tipo de cambio para la conversión entre colones y dólares.
 *                   Debe ser un valor positivo y mayor que cero.
 * @param db Puntero a la base de datos SQLite donde se encuentran las cuentas.
 * 
 * 
 * @note La función registra un movimiento en la tabla de movimientos, incluso en caso de errores 
 *       como fondos insuficientes.
 */
void realizadTransferencia(const int& idOrigen, const int& idDestino, const double& TipoCambio, sqlite3* db) {
    int colones = 100; 
    int dolares = 200; 
    double transfer; 
    string idOrig = std::to_string(idOrigen); // Convertir a string
    string idDest = std::to_string(idDestino); //convertir a string
    string digOrig = idOrig.substr(0, 3); // extraer los primeros 3 caracteres
    string digDest = idDest.substr(0, 3); // Extraer los primeros 3 caracteres

    cout << "Ingrese la cantidad de dinero que desea transferir." << endl;
    cin >> transfer; 

    // Definiendo consulta SQL según el tipo de cuenta
    string query;

    if (std::stoi(digOrig) == colones && stoi(digDest) == colones) {
        // Comprobar que la cuenta de origen no quede en negativo
        string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Clones WHERE id = ?";
        sqlite3_stmt* checkStmt;
        sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
        sqlite3_bind_text(checkStmt, 1, idOrig.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            double currentBalance = sqlite3_column_double(checkStmt, 0);
            if (currentBalance - transfer < 0) {
                cout << "No se puede retirar esa cantidad de dinero. Fondos insuficientes." << endl;
                ostringstream detalle;
                detalle << "Transferencia de " << transfer << "colones, no realizada por fondos insuficientes"<< endl ;
                // Convertir a detalle a string
                const string detalleTexto = detalle.str();

                AgregarMov(idOrigen, detalleTexto, db); 
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
                ostringstream detalle;
                detalle << "Transferencia de " << transfer << "dolares, no realizada por fondos insuficientes"<< endl ;
                // Convertir a detalle a string
                const string detalleTexto = detalle.str();
                sqlite3_finalize(checkStmt);
                AgregarMov(idOrigen, detalleTexto, db);
                return;
            }
        }
        sqlite3_finalize(checkStmt);

        query ="UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?; "
                "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero + ? WHERE id = ?; ";

    } else if (stoi(digOrig) == colones && stoi(digDest) == dolares) {
        // Comprobar que la cuenta de origen no quede en negativo
        string checkQuery = "SELECT cantidad_dinero FROM Cuenta_Clones WHERE id = ?";
        sqlite3_stmt* checkStmt;
        sqlite3_prepare_v2(db, checkQuery.c_str(), -1, &checkStmt, 0);
        sqlite3_bind_text(checkStmt, 1, idOrig.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            double currentBalance = sqlite3_column_double(checkStmt, 0);
            if (currentBalance - transfer < 0) {
                cout << "No se puede retirar esa cantidad de dinero. Fondos insuficientes." << endl;
                ostringstream detalle;
                detalle << "Transferencia de " << transfer << "colones, no realizada por fondos insuficientes" << endl;
                // Convertir a detalle a string
                const string detalleTexto = detalle.str();
                AgregarMov(idOrigen, detalleTexto, db);
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
                ostringstream detalle;
                detalle << "Transferencia de " << transfer << "dolares, no realizada por fondos insuficientes"<< endl ;
                // Convertir a detalle a string
                const string detalleTexto = detalle.str();
                AgregarMov(idOrigen, detalleTexto, db);
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
            ostringstream detalle;
                detalle << "Transferencia de " << transfer <<", hacia la cuenta " << idDestino << endl;
                // Convertir a detalle a string
                const string detalleTexto = detalle.str();
                AgregarMov(idOrigen, detalleTexto, db);
        }   
        sqlite3_finalize(stmt);

        return;
    }
}


/**
 * @brief Realiza un abono a un préstamo desde una cuenta bancaria.
 * 
 * La función realiza un abono desde una cuenta bancaria hacia un préstamo, 
 * manejando conversiones de moneda según el tipo de cambio proporcionado y 
 * asegurándose de que la cuenta no quede con saldo negativo. También calcula 
 * y actualiza la cuota mensual del préstamo tras el abono.
 * 
 * @param idPrestamo ID del préstamo al que se desea abonar.
 * @param TipoCambio Tipo de cambio para la conversión entre colones y dólares.
 *                   Debe ser un valor positivo mayor que cero.
 * @param idCuenta ID de la cuenta desde la cual se realizará el abono.
 * @param db Puntero a la base de datos SQLite que contiene la información de préstamos y cuentas.
 * 
 * @note Los primeros tres dígitos del ID de la cuenta determinan el tipo de cuenta:
 *       - `100`: Cuenta en colones.
 *       - `200`: Cuenta en dólares.
 * 
 * @note Si la cuenta y el préstamo están en diferentes monedas, se realiza una conversión 
 *       utilizando el tipo de cambio proporcionado.
 * 
 * @note La función redondea los valores monetarios a dos decimales para evitar errores de precisión.
 * 
 * 
 * 
 * @see AgregarMov para registrar movimientos en la base de datos.
 */
void realizarAbono(const int& idPrestamo, const double& TipoCambio, const int& idCuenta, sqlite3* db) {
    // Determinar el tipo de cuenta según el prefijo del ID
    string idCuentaStr = to_string(idCuenta);
    string cuentaPrefix = idCuentaStr.substr(0, 3);

    int cuentaEsColones = (cuentaPrefix == "100");
    int cuentaEsDolares = (cuentaPrefix == "200");

    // Consultar el tipo de préstamo (colones o dólares)
    string queryPrestamo;
    if (cuentaEsColones) {
        queryPrestamo = "SELECT saldo_restante, cuotas_pagadas, meses, intereses FROM Prestamos_Colones WHERE id_prestamo = ?";
    } else if (cuentaEsDolares) {
        queryPrestamo = "SELECT saldo_restante, cuotas_pagadas, meses, intereses FROM Prestamos_Dolares WHERE id_prestamo = ?";
    } else {
        cerr << "El ID de la cuenta no corresponde a un tipo válido." << endl;
        return;
    }

    double montoRestante;
    int cuotasPagadas;
    int plazoMeses;
    double intereses;

    sqlite3_stmt* stmtPrestamo;
    if (sqlite3_prepare_v2(db, queryPrestamo.c_str(), -1, &stmtPrestamo, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta del préstamo: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_int(stmtPrestamo, 1, idPrestamo);

    if (sqlite3_step(stmtPrestamo) == SQLITE_ROW) {
        montoRestante = sqlite3_column_double(stmtPrestamo, 0);
        cuotasPagadas = sqlite3_column_int(stmtPrestamo, 1);
        plazoMeses = sqlite3_column_int(stmtPrestamo, 2);
        intereses = sqlite3_column_int(stmtPrestamo, 3);
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

    // Se redondea la cifra ingresada y se limita a dos decimales
    montoAbono = round(montoAbono * 100) / 100;

    // Validar y realizar el abono
    if (cuentaEsColones && montoRestante > 0) {
        // La cuenta y el préstamo están en colones
        if (saldoCuenta - montoAbono < 1) {
            cerr << "No se puede realizar el abono. La cuenta no puede quedar con saldo menor que 1." << endl;
            ostringstream detalle;
            detalle << "Abono de " << montoAbono << "colones, no realizada por fondos insuficientes" << endl;
            // Convertir a detalle a string
            const string detalleTexto = detalle.str();
            AgregarMov(idCuenta, detalleTexto, db);
            return;
        }
    } else if (cuentaEsDolares && montoRestante > 0) {
        // La cuenta y el préstamo están en dólares
        if (saldoCuenta - montoAbono < 1) {
            cerr << "No se puede realizar el abono. La cuenta no puede quedar con saldo menor que 1." << endl;
            ostringstream detalle;
            detalle << "Abono de " << montoAbono << "dolares, no realizada por fondos insuficientes" << endl;
            // Convertir a detalle a string
            const string detalleTexto = detalle.str();
            AgregarMov(idCuenta, detalleTexto, db);
            return;
        }
    } else if (cuentaEsDolares && montoRestante > 0) {
        // Conversión de dólares a colones
        double montoConvertido = montoAbono * (TipoCambio);
        if (saldoCuenta - montoConvertido < 1) {
            cerr << "No se puede realizar el abono. La cuenta no puede quedar con saldo menor que 1 después de la conversión." << endl;
            ostringstream detalle;
            detalle << "Abono de " << montoAbono << "dolares, no realizada por fondos insuficientes"<< endl ;
            // Convertir a detalle a string
            const string detalleTexto = detalle.str();
            AgregarMov(idCuenta, detalleTexto, db);
            return;
        }
        montoAbono = montoConvertido;
    } else if (cuentaEsColones && montoRestante > 0) {
        // Conversión de colones a dólares
        double montoConvertido = montoAbono / (TipoCambio);
        if (saldoCuenta - montoConvertido < 1) {
            cerr << "No se puede realizar el abono. La cuenta no puede quedar con saldo menor que 1 después de la conversión." << endl;
            ostringstream detalle;
            detalle << "Abono de " << montoAbono << "colones, no realizada por fondos insuficientes"<< endl ;
            // Convertir a detalle a string
            const string detalleTexto = detalle.str();
            AgregarMov(idCuenta, detalleTexto, db);
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

    // Se calcula la cuota mensual actualizada con el nuevo capital
    // Cuota Mensual = (Capital * Taza Interes Mensual) / (1 - (1 + Taza Interes Mensual)^(-Plazo en Meses));
    double interesMensual = (intereses) / 12 / 100;
    double cuotaMensual = ((montoRestante - montoAbono) * interesMensual) / (1 - pow(1 + interesMensual, -(plazoMeses - cuotasPagadas)));
    // Se redondea la cifra y se limita a dos decimales
    cuotaMensual = round(cuotaMensual * 100) / 100;

    // Actualizar monto restante del préstamo y la nueva cuota mensual
    string queryUpdatePrestamo = cuentaEsColones
        ? "UPDATE Prestamos_Colones SET saldo_restante = saldo_restante - ?, monto_por_cuota = ? WHERE id_prestamo = ?"
        : "UPDATE Prestamos_Dolares SET saldo_restante = saldo_restante - ?, monto_por_cuota = ? WHERE id_prestamo = ?";
    sqlite3_stmt* stmtUpdatePrestamo;
    if (sqlite3_prepare_v2(db, queryUpdatePrestamo.c_str(), -1, &stmtUpdatePrestamo, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta de actualización del préstamo: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_double(stmtUpdatePrestamo, 1, montoAbono);
    sqlite3_bind_double(stmtUpdatePrestamo, 2, cuotaMensual);
    sqlite3_bind_int(stmtUpdatePrestamo, 3, idPrestamo);

    if (sqlite3_step(stmtUpdatePrestamo) != SQLITE_DONE) {
        cerr << "Error al ejecutar la actualización del préstamo: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmtUpdatePrestamo);
        return;
    }
    sqlite3_finalize(stmtUpdatePrestamo);

    cout << "Abono realizado con éxito. Monto descontado: " << montoAbono << ". Préstamo actualizado correctamente." << endl;
    ostringstream detalle;
    detalle << "Abono de " << montoAbono << ", no realizada por fondos insuficientes"<< endl ;
    // Convertir a detalle a string
    const string detalleTexto = detalle.str();
    AgregarMov(idCuenta, detalleTexto, db);
}

/**
 * @brief Realiza el pago de una cuota de préstamo desde una cuenta especificada.
 * 
 * Esta función consulta los detalles de un préstamo, valida que la cuenta tenga fondos suficientes
 * para realizar el pago y actualiza tanto la cuenta como el préstamo correspondiente.
 * Además, se asegura de que el monto a pagar esté correctamente calculado, tomando en cuenta el tipo de cuenta
 * (Colones o Dólares) y el tipo de cambio si es necesario.
 *
 * @param idPrestamo ID único del préstamo a pagar.
 * @param idCuenta ID único de la cuenta desde la cual se realiza el pago.
 * @param TipoCambio Tipo de cambio actual entre Colones y Dólares (se usa si la cuenta es en una moneda y el préstamo en otra).
 * @param db Puntero a la base de datos SQLite donde se encuentran las tablas de cuentas y préstamos.
 * 
 * @note El tipo de cuenta se determina a partir de los primeros 3 caracteres del ID de la cuenta:
 *       - "100" es una cuenta en Colones.
 *       - "200" es una cuenta en Dólares.
 * 
 * @note La función no realiza el pago si no hay fondos suficientes en la cuenta para cubrir el monto de la cuota,
 *          o si la cuota no se puede pagar debido a un error en la consulta de la base de datos.
 * 
 */
void pagarCuota(const int& idPrestamo, const int& idCuenta, const double& TipoCambio, sqlite3* db) {
    // Determinar el tipo de cuenta según el prefijo del ID
    string idCuentaStr = to_string(idCuenta);
    string cuentaPrefix = idCuentaStr.substr(0, 3);

    int cuentaEsColones = (cuentaPrefix == "100");
    int cuentaEsDolares = (cuentaPrefix == "200");

    // Consultar los datos del préstamo
    string queryPrestamo;
    if (cuentaEsColones) {
        queryPrestamo = "SELECT monto_por_cuota, intereses, cuotas_pagadas, saldo_restante, meses FROM Prestamos_Colones WHERE id_prestamo = ?";
    } else if (cuentaEsDolares) {
        queryPrestamo = "SELECT monto_por_cuota, intereses, cuotas_pagadas, saldo_restante, meses FROM Prestamos_Dolares WHERE id_prestamo = ?";
    } else {
        cerr << "El ID de la cuenta no corresponde a un tipo válido." << endl;
        return;
    }

    double montoTotalCuota;
    double intereses;
    int cuotasPagadas;
    double saldoRestante;
    int plazoMeses;

    sqlite3_stmt* stmtPrestamo;
    if (sqlite3_prepare_v2(db, queryPrestamo.c_str(), -1, &stmtPrestamo, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta del préstamo: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_int(stmtPrestamo, 1, idPrestamo);

    if (sqlite3_step(stmtPrestamo) == SQLITE_ROW) {
        montoTotalCuota = sqlite3_column_double(stmtPrestamo, 0);
        intereses = sqlite3_column_double(stmtPrestamo, 1);
        cuotasPagadas = sqlite3_column_int(stmtPrestamo, 2);
        saldoRestante = sqlite3_column_double(stmtPrestamo, 3);
        plazoMeses = sqlite3_column_int(stmtPrestamo, 4);
    } else {
        cerr << "No se encontró el préstamo con ID " << idPrestamo << endl;
        sqlite3_finalize(stmtPrestamo);
        return;
    }
    sqlite3_finalize(stmtPrestamo);

    // Se redondea el monto por cuota extraido y se limita a dos decimales
    montoTotalCuota = round(montoTotalCuota * 100) / 100;

    // Si es la ultima cuota del prestamo
    if ((plazoMeses - cuotasPagadas) == 1 ) {
        // Se re define la cuota mensual actualizada
        double interesMensual = (intereses) / 12 / 100;
        montoTotalCuota = (saldoRestante * interesMensual) / (1 - pow(1 + interesMensual, -1));
    }

    // Se calcula la Cuota Interes Mensual
    double cuotaInteresMensual = (saldoRestante * (intereses / 100)) / 12;
    cuotaInteresMensual = round(cuotaInteresMensual * 100) / 100; 

    // Se calcula el rebajo al prestamo sin la cuota de interes mensual
    double montoCuotaSinInteres = montoTotalCuota - cuotaInteresMensual;
    montoCuotaSinInteres = round(montoCuotaSinInteres * 100) / 100;


    // Se calcula el saldo restante
    saldoRestante = saldoRestante - montoCuotaSinInteres;
    saldoRestante = round(saldoRestante * 100) / 100;


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

    // Realizar validaciones según los tipos de cuenta y préstamo
    if (cuentaEsColones && saldoCuenta < montoTotalCuota) {
        cerr << "Saldo insuficiente para pagar la cuota. Se requieren " << montoTotalCuota << " colones." << endl;
        ostringstream detalle;
        detalle << "Pago de cuota de " << montoTotalCuota << " colones, no realizada por fondos insuficientes" << endl ;
        // Convertir a detalle a string
        const string detalleTexto = detalle.str();
        AgregarMov(idCuenta, detalleTexto, db);
        return;
    } else if (cuentaEsDolares && saldoCuenta < montoTotalCuota) {
        cerr << "Saldo insuficiente para pagar la cuota. Se requieren " << montoTotalCuota << " dólares." << endl;
        ostringstream detalle;
        detalle << "Pago de cuota de " << montoTotalCuota << " dolares, no realizada por fondos insuficientes" << endl ;
        // Convertir a detalle a string
        const string detalleTexto = detalle.str();
        AgregarMov(idCuenta, detalleTexto, db);
        return;
    } else if (cuentaEsColones && montoTotalCuota / TipoCambio > saldoCuenta) {
        cerr << "Saldo insuficiente para pagar la cuota convertida en dólares." << endl;
        ostringstream detalle;
        detalle << "Pago de cuota de " << montoTotalCuota << " colones, no realizada por fondos insuficientes" << endl;
        // Convertir a detalle a string
        const string detalleTexto = detalle.str();
        AgregarMov(idCuenta, detalleTexto, db);
        return;
    } else if (cuentaEsDolares && montoTotalCuota * TipoCambio > saldoCuenta) {
        cerr << "Saldo insuficiente para pagar la cuota convertida en colones." << endl;
        ostringstream detalle;
        detalle << "Pago de cuota de " << montoTotalCuota << " dolares, no realizada por fondos insuficientes" << endl;
        // Convertir a detalle a string
        const string detalleTexto = detalle.str();
        AgregarMov(idCuenta, detalleTexto, db);
        return;
    }

    // Actualizar el saldo de la cuenta
    double montoADebitar = (cuentaEsColones) ? montoTotalCuota : montoTotalCuota;
    string queryUpdateCuenta = cuentaEsColones
        ? "UPDATE Cuenta_Colones SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?"
        : "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero - ? WHERE id = ?";
    sqlite3_stmt* stmtUpdateCuenta;
    if (sqlite3_prepare_v2(db, queryUpdateCuenta.c_str(), -1, &stmtUpdateCuenta, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta de actualización de cuenta: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_double(stmtUpdateCuenta, 1, montoADebitar);
    sqlite3_bind_int(stmtUpdateCuenta, 2, idCuenta);

    if (sqlite3_step(stmtUpdateCuenta) != SQLITE_DONE) {
        cerr << "Error al ejecutar la actualización de la cuenta: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmtUpdateCuenta);
        return;
    }
    sqlite3_finalize(stmtUpdateCuenta);

    // Actualizar el número de cuotas pagadas, saldo restante por pagar y los intereses abonados
    if (cuotasPagadas < plazoMeses) {
        string queryUpdatePrestamo = cuentaEsColones
            ? "UPDATE Prestamos_Colones SET cuotas_pagadas = cuotas_pagadas + 1, saldo_restante = ?, intereses_abonados = intereses_abonados + ? WHERE id_prestamo = ?"
            : "UPDATE Prestamos_Dolares SET cuotas_pagadas = cuotas_pagadas + 1, saldo_restante = ?, intereses_abonados = intereses_abonados + ? WHERE id_prestamo = ?";
        
        sqlite3_stmt* stmtUpdatePrestamo;
        if (sqlite3_prepare_v2(db, queryUpdatePrestamo.c_str(), -1, &stmtUpdatePrestamo, 0) != SQLITE_OK) {
            cerr << "Error al preparar la consulta de actualización del préstamo: " << sqlite3_errmsg(db) << endl;
            return;
        }
        sqlite3_bind_double(stmtUpdatePrestamo, 1, saldoRestante);
        sqlite3_bind_double(stmtUpdatePrestamo, 2, cuotaInteresMensual);
        sqlite3_bind_int(stmtUpdatePrestamo, 3, idPrestamo);

        if (sqlite3_step(stmtUpdatePrestamo) != SQLITE_DONE) {
            cerr << "Error al ejecutar la actualización del préstamo: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmtUpdatePrestamo);
            return;
        }
        sqlite3_finalize(stmtUpdatePrestamo);

        cout << "Pago de cuota realizado con éxito. Se descontaron " << montoTotalCuota << " de la cuenta y se actualizó el préstamo." << endl;
        ostringstream detalle;
        detalle << "Pago de cuota de " << montoTotalCuota << endl;
        // Convertir a detalle a string
        const string detalleTexto = detalle.str();
        AgregarMov(idCuenta, detalleTexto, db);
    } else {
        cerr << "No hay cuotas pendientes para el préstamo." << endl;
    }
}


/**
 * @brief Registra el permiso de salida del país para una cuenta.
 * 
 * Esta función actualiza el campo `salida_pais` de la cuenta en la base de datos para indicar
 * que la cuenta tiene permiso para realizar una salida del país. El tipo de cuenta (Colones o Dólares)
 * se determina según los primeros tres dígitos del ID de la cuenta.
 *
 * @param idCuenta ID de la cuenta para la cual se desea registrar el permiso de salida del país.
 * @param db Puntero a la base de datos SQLite donde se encuentran las cuentas.
 * 
 * @note Si el ID de la cuenta comienza con "100", se considera una cuenta en Colones.
 *       Si comienza con "200", se considera una cuenta en Dólares.
 * 
 */
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


/**
 * @brief Consulta el PIN de una cuenta en la base de datos.
 * 
 * Esta función busca el PIN de la cuenta especificada, dependiendo de si la cuenta es en Colones
 * o en Dólares, utilizando los primeros tres dígitos del ID de la cuenta para determinar el tipo de cuenta.
 * Si el ID comienza con "100", se consulta en la tabla de cuentas en colones, si comienza con "200",
 * se consulta en la tabla de cuentas en dólares.
 *
 * @param idCuenta ID de la cuenta para la cual se desea consultar el PIN.
 * @param db Puntero a la base de datos SQLite donde se encuentran las cuentas.
 * 
 * @note Si el ID de la cuenta comienza con "100", se consulta la tabla `Cuenta_Colones`.
 *       Si comienza con "200", se consulta la tabla `Cuenta_Dolares`.
 * 
 */
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


/**
 * @brief Consulta el CVV (o PIN) de una cuenta en la base de datos.
 * 
 * Esta función busca el CVV de la cuenta especificada, dependiendo de si la cuenta es en Colones
 * o en Dólares, utilizando los primeros tres dígitos del ID de la cuenta para determinar el tipo de cuenta.
 * Si el ID comienza con "100", se consulta en la tabla de cuentas en colones, si comienza con "200",
 * se consulta en la tabla de cuentas en dólares.
 *
 * @param idCuenta ID de la cuenta para la cual se desea consultar el CVV.
 * @param db Puntero a la base de datos SQLite donde se encuentran las cuentas.
 * 
 * @note Si el ID de la cuenta comienza con "100", se consulta la tabla `Cuenta_Colones`.
 *       Si comienza con "200", se consulta la tabla `Cuenta_Dolares`.
 */
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


/**
 * @brief Imprime el estado de cuenta de un usuario, mostrando los detalles y fechas de los movimientos registrados.
 * 
 * Esta función consulta la base de datos para obtener los movimientos de la cuenta, ya sea en colones o en dólares,
 * según el prefijo del ID de la cuenta. Los resultados se imprimen en un formato tabular con los detalles de cada movimiento,
 * incluyendo la fecha en que ocurrió.
 * 
 * @param idCuenta ID de la cuenta cuya información de movimientos se quiere imprimir.
 * @param db Puntero a la base de datos SQLite donde se encuentran los registros de los movimientos.
 * 
 * @note Si el ID de la cuenta comienza con "100", se consulta la tabla `Movimientos_Colones`.
 *       Si comienza con "200", se consulta la tabla `Movimientos_dolares`.
 *       Si el prefijo del ID no corresponde a estas tablas, se imprime un mensaje indicando que la cuenta no es válida.
 * 
 */
void imprimirEstadoCuenta(const int& idCuenta, sqlite3* db) {
    // Convertir el id de cuenta a string para identificar si es de colones o dólares
    string idCuentaStr = to_string(idCuenta);
    string cuentaPrefix = idCuentaStr.substr(0, 3);

    // Determinar la tabla correspondiente
    string query;
    if (cuentaPrefix == "100") {
        query = "SELECT detalle, fecha FROM Movimientos_Colones WHERE id_cuenta = ?;";
    } else if (cuentaPrefix == "200") {
        query = "SELECT detalle, fecha FROM Movimientos_dolares WHERE id_cuenta = ?;";
    } else {
        cout << "Cuenta no válida." << endl;
        return;
    }

    // Preparar la consulta SQL
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Vincular el id de la cuenta al primer parámetro de la consulta
    sqlite3_bind_int(stmt, 1, idCuenta);

    // Comenzamos a imprimir la cabecera
    cout << "**********************************" << endl;
    cout << "*     Estado de Cuenta          *" << endl;
    cout << "**********************************" << endl;
    cout << "* ID Cuenta: " << idCuenta << "                      *" << endl;
    cout << "**********************************" << endl;
    cout << setw(30) << left << "Detalle" << setw(20) << "Fecha" << endl;
    cout << "----------------------------------" << endl;

    // Recorrer los resultados de la consulta
    bool hayMovimientos = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hayMovimientos = true;

        // Obtener los valores de cada movimiento
        const unsigned char* detalle = sqlite3_column_text(stmt, 0);
        const unsigned char* fecha = sqlite3_column_text(stmt, 1);

        // Imprimir el movimiento de manera estética
        cout << setw(30) << left << detalle << setw(20) << (char*)fecha << endl;
    }

    // Si no hay movimientos, informar al usuario
    if (!hayMovimientos) {
        cout << "No se encontraron movimientos para esta cuenta." << endl;
    }

    // Cerrar el statement
    sqlite3_finalize(stmt);
    
    // Imprimir una línea final
    cout << "**********************************" << endl;
}


/**
 * @brief Agrega un nuevo movimiento a la base de datos de acuerdo con el tipo de cuenta.
 * 
 * Esta función inserta un nuevo registro en la tabla correspondiente (`Movimientos_Colones` o `Movimientos_dolares`) 
 * para el ID de cuenta proporcionado. Dependiendo del prefijo del ID de cuenta (100 para colones, 200 para dólares), 
 * la consulta SQL se dirige a la tabla adecuada. El movimiento incluye el detalle proporcionado.
 * 
 * @param id ID de la cuenta a la que se registrará el movimiento.
 * @param Detalle Descripción detallada del movimiento que se desea registrar.
 * @param db Puntero a la base de datos SQLite donde se insertará el movimiento.
 * 
 * @note Si el prefijo del ID de la cuenta no es "100" ni "200", se imprime un mensaje de error indicando que el ID 
 *       no corresponde a un tipo de cuenta válido.
 */
void AgregarMov(const int& id, const string& Detalle, sqlite3* db) {
    // Convertir el id de cuenta a string para poder analizar el prefijo
    string idStr = to_string(id);
    string cuentaPrefix = idStr.substr(0, 3);

    // Determinar si es cuenta en colones o en dólares
    bool cuentaEsColones = (cuentaPrefix == "100");
    bool cuentaEsDolares = (cuentaPrefix == "200");

    // Crear la consulta SQL para insertar el movimiento en la tabla correspondiente
    string query;
    if (cuentaEsColones) {
        query = "INSERT INTO Movimientos_Colones (id_cuenta, detalle) VALUES (?, ?);";
    } else if (cuentaEsDolares) {
        query = "INSERT INTO Movimientos_dolares (id_cuenta, detalle) VALUES (?, ?);";
    } else {
        cerr << "El ID de cuenta no corresponde a un tipo válido." << endl;
        return;
    }

    // Preparar el statement SQL para la inserción
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Vincular los parámetros de la consulta
    sqlite3_bind_int(stmt, 1, id);                        // Vincular el ID de la cuenta
    sqlite3_bind_text(stmt, 2, Detalle.c_str(), -1, SQLITE_STATIC);  // Vincular el detalle del movimiento

    // Ejecutar la consulta
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Movimiento registrado correctamente." << endl;
    }

    // Limpiar el statement
    sqlite3_finalize(stmt);
}

/**
 * @brief Muestra el menú de opciones de atención al cliente y maneja la selección del usuario.
 * 
 * Esta función muestra un menú con diversas opciones de servicios disponibles para el cliente. Dependiendo de la opción 
 * seleccionada por el usuario, se ejecuta una operación específica, como realizar un depósito, retiro, transferencia, 
 * entre otros. La función permite que el cliente interactúe con el sistema realizando diversas consultas y transacciones 
 * en base a su cuenta.
 * 
 * @param idCuenta ID de la cuenta del cliente para las operaciones relacionadas.
 * @param TipoCambio Tipo de cambio que se puede usar en servicios de cambio de divisa.
 * @param db Puntero a la base de datos SQLite que contiene la información de la cuenta y las transacciones.
 * 
 * @note Se valida que la opción ingresada esté en el rango válido (1-10). Si el usuario ingresa una opción fuera de ese 
 *       rango o con un formato inválido, se lanzará una excepción.
 */
void showMenuAC(const int& idCuenta, const double& TipoCambio, sqlite3* db) {
    int opcion;

    do {
        try {
            cout << "Ingrese el servicio que desea recibir: " << endl;
            cout << "1. Realizar depósito." << endl;
            cout << "2. Realizar retiro." << endl;
            cout << "3. Realizar transferencia." << endl;
            cout << "4. Realizar abono." << endl;
            cout << "5. Pagar cuota." << endl;
            cout << "6. Registrar salida del país." << endl;
            cout << "7. Consultar PIN." << endl;
            cout << "8. Consultar CVV." << endl;
            cout << "9. Imprimir estado de cuenta." << endl;
            cout << "10. Regresar al menú anterior." << endl;
            cout << "Ingrese su selección: ";
            cin >> opcion;

            // Validar entrada
            if (cin.fail() || opcion < 1 || opcion > 10) {
                throw invalid_argument("Opción inválida. Debe ingresar un número entre 1 y 10.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer

            switch (opcion) {
                case 1:
                    realizadDeposito(idCuenta, db);
                    break;
                case 2:
                    realizadRetiro(idCuenta, db);
                    break;
                case 3: {
                    int idDest;
                    cout << "Ingrese el número de cuenta al que desea realizar la transferencia: ";
                    cin >> idDest;
                    if (cin.fail()) {
                        throw invalid_argument("ID de cuenta destino no válido. Debe ser un número entero.");
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer
                    realizadTransferencia(idCuenta, idDest, TipoCambio, db);
                    break;
                }
                case 4: {
                    int idPrestamo;
                    cout << "Ingrese el número de depósito: ";
                    cin >> idPrestamo;
                    if (cin.fail()) {
                        throw invalid_argument("Número de depósito no válido. Debe ser un número entero.");
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer
                    realizarAbono(idPrestamo, TipoCambio, idCuenta, db);
                    break;
                }
                case 5: {
                    int idPrestamo;
                    cout << "Ingrese el número de préstamo: ";
                    cin >> idPrestamo;
                    if (cin.fail()) {
                        throw invalid_argument("Número de préstamo no válido. Debe ser un número entero.");
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer
                    pagarCuota(idPrestamo, idCuenta, TipoCambio, db);
                    break;
                }
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
                    cout << "Regresando al menú anterior..." << endl;
                    break;
                default:
                    cout << "Opción no válida. Por favor, intente de nuevo." << endl;
            }
        } catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer
        }
    } while (opcion != 10);
}

