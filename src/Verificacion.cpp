#include "Verificacion.hpp"
#include <iostream>
#include <sqlite3.h>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include "AbrirDB.hpp"
#include "Hash.hpp"

using namespace std;


// Función para verificar si una cuenta existe en una tabla específica
bool cuentaValida(sqlite3* db, const string& tabla, int id, const string& Password) {
    string hashedPassword = hashSHA256(Password);
    string sql = "SELECT COUNT(*) FROM " + tabla + " WHERE id = " + to_string(id) + " AND Password = '" + hashedPassword + "';";
    sqlite3_stmt* stmt;
    bool esValida = false;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            esValida = true;
        }
    } else {
        cerr << "Error al verificar la cuenta: " << sqlite3_errmsg(db) << endl;
    }
    sqlite3_finalize(stmt);
    return esValida;
}

// Función para verificar una cuenta en ambas tablas
bool verificarCuenta(sqlite3* db, int id, const string& password) {
    // Verificar en la tabla de Cuenta_Colones
    if (cuentaValida(db, "Cuenta_Colones", id, password)) {
        return true;
    }
    // Verificar en la tabla de Cuenta_Dolares
    if (cuentaValida(db, "Cuenta_Dolares", id, password)) {
        return true;
    }

    cout << "Cuenta no encontrada o contraseña incorrecta.\n";
    return false;
}

