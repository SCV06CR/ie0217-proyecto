#include <iostream>
#include <sqlite3.h>
#include <string>
#include <openssl/sha.h>  // Necesita OpenSSL para SHA-256
#include <sstream>
#include <iomanip>
#include "AbrirDB.hpp"

using namespace std;

// Función para ejecutar comandos SQL sin retorno
void ejecutarSQL(sqlite3* db, const char* sql) {
    char* mensajeError;
    if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
        cerr << "Error al ejecutar SQL: " << mensajeError << endl;
        sqlite3_free(mensajeError);
    } else {
        cout << "Operacion Exitosa." << endl;
    }
}

// Función para hacer hash de la contraseña usando SHA-256
std::string hashSHA256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Función para verificar si una cuenta existe y si la contraseña es correcta
bool cuentaValida(sqlite3* db, const std::string& tabla, int id, const std::string& Password) {
    std::string hashedPassword = hashSHA256(Password);
    std::string sql = "SELECT COUNT(*) FROM " + tabla + " WHERE id = " + std::to_string(id) + " AND Password = '" + hashedPassword + "';";
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

// Función para borrar una cuenta en una tabla (Cuenta_Colones o Cuenta_Dolares) con verificación de contraseña
void borrarCuenta(sqlite3* db) {
    int id;
    string tabla, Password;

    cout << "Ingrese el ID de la cuenta que desea borrar: ";
    cin >> id;
    cin.ignore();

    // Selección de la tabla
    cout << "Ingrese el tipo de moneda de la cuenta (Cuenta_Colones o Cuenta_Dolares): ";
    getline(cin, tabla);

    // Verificar si la tabla ingresada es válida
    if (tabla != "Cuenta_Colones" && tabla != "Cuenta_Dolares") {
        cerr << "Tabla invalida. Debe ser 'Cuenta_Colones' o 'Cuenta_Dolares'." << endl;
        return;
    }

    // Solicitar y hashear la contraseña
    cout << "Ingrese la password de la cuenta: ";
    getline(cin, Password);

    // Verificar si la cuenta existe y si la contraseña es correcta
    if (!cuentaValida(db, tabla, id, Password)) {
        cout << "La cuenta con ID " << id << " no existe o la contraseña es incorrecta." << endl;
        return;
    }

    // Borrar la cuenta si la verificación es exitosa
    std::string sql = "DELETE FROM " + tabla + " WHERE id = " + std::to_string(id) + ";";
    ejecutarSQL(db, sql.c_str());

    cout << "Cuenta con ID " << id << " eliminada de " << tabla << "." << endl;
}

int main() {
    // Abrir la base de datos
    sqlite3* db = abrirBaseDatos("SistemaBancario.db");

    if (db) {
        // Borrar una cuenta
        borrarCuenta(db);

        // Cerrar la base de datos
        sqlite3_close(db);
    }

    return 0;
}
