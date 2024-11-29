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
/**
 * @brief Verifica si una cuenta es válida en una tabla específica.
 * 
 * Esta función consulta la base de datos para verificar si una cuenta con el ID y la contraseña proporcionados existe en 
 * la tabla especificada. La contraseña se compara utilizando su hash SHA-256.
 * 
 * @param db Puntero a la base de datos SQLite.
 * @param tabla El nombre de la tabla donde se buscará la cuenta (por ejemplo, "Cuenta_Colones" o "Cuenta_Dolares").
 * @param id El ID de la cuenta a verificar.
 * @param Password La contraseña asociada a la cuenta, que será convertida a su hash SHA-256.
 * @return true Si la cuenta con el ID y contraseña existe en la tabla.
 * @return false Si la cuenta no existe o la contraseña no es válida.
 */
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
/**
 * @brief Verifica si una cuenta es válida en las tablas "Cuenta_Colones" y "Cuenta_Dolares".
 * 
 * Esta función solicita al usuario un ID de cuenta de 9 dígitos y verifica si existe en las tablas de cuentas 
 * en Colones y Dólares. La contraseña proporcionada se verifica utilizando su hash SHA-256.
 * 
 * @param db Puntero a la base de datos SQLite.
 * @param id El ID de la cuenta a verificar.
 * @param password La contraseña asociada a la cuenta.
 * @return true Si la cuenta es válida en alguna de las tablas.
 * @return false Si la cuenta no existe o la contraseña es incorrecta.
 */
bool verificarCuenta(sqlite3* db, int id, const string& password) {

    // Bucle para solicitar el ID hasta que tenga 9 dígitos
    while (to_string(id).length() != 9) {
        cerr << "Error: El ID debe tener exactamente 9 dígitos." << endl;
        cout << "Por favor, ingrese un ID válido de 9 dígitos: ";
        cin >> id;
    }

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

// Función para verificar si un Prestamo existe en una tabla específica
/**
 * @brief Verifica si un préstamo es válido en una tabla específica.
 * 
 * Esta función consulta la base de datos para verificar si un préstamo con el ID proporcionado existe en 
 * la tabla de préstamos correspondiente.
 * 
 * @param db Puntero a la base de datos SQLite.
 * @param tabla El nombre de la tabla donde se buscará el préstamo (por ejemplo, "Prestamos_Colones" o "Prestamos_Dolares").
 * @param idPrestamo El ID del préstamo a verificar.
 * @return true Si el préstamo existe en la tabla.
 * @return false Si el préstamo no existe.
 */
bool prestamoValido(sqlite3* db, const string& tabla, string idPrestamo) {
    string sql = "SELECT COUNT(*) FROM " + tabla + " WHERE id_prestamo = " + idPrestamo + ";";
    sqlite3_stmt* stmt;
    bool esValida = false;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            esValida = true;
        }
    } else {
        cerr << "Error al verificar el ID del Prestamo: " << sqlite3_errmsg(db) << endl;
    }
    sqlite3_finalize(stmt);
    return esValida;
}

// Función para verificar un prestamo en ambas tablas
/**
 * @brief Verifica si un préstamo es válido en las tablas "Prestamos_Colones" y "Prestamos_Dolares".
 * 
 * Esta función solicita al usuario un ID de préstamo de 8 dígitos y verifica si existe en las tablas de préstamos 
 * en Colones y Dólares.
 * 
 * @param db Puntero a la base de datos SQLite.
 * @param idPrestamo El ID del préstamo a verificar.
 * @return true Si el préstamo es válido en alguna de las tablas.
 * @return false Si el préstamo no existe.
 */
bool verificarPrestamo(sqlite3* db, string &idPrestamo) {
    
    // Bucle para solicitar el ID hasta que tenga 9 dígitos
    if (idPrestamo.length() != 8) {
        cerr << "Error: El ID debe tener exactamente 8 dígitos." << endl;
        cout << "Por favor, ingrese un ID válido de 8 dígitos\n";
        return false;
    }

    // Verificar en la tabla de Cuenta_Colones
    if (prestamoValido(db, "Prestamos_Colones", idPrestamo)) {
        return true;
    }
    // Verificar en la tabla de Cuenta_Dolares
    if (prestamoValido(db, "Prestamos_Dolares", idPrestamo)) {
        return true;
    }

    cout << "Prestamo no encontrado\n";
    return false;
}

