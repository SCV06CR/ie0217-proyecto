#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std;

// Funcion que ejecuta consultas sin retorno
void ejecutarSQL(sqlite3* db, const char* sql) {
    char* mensajeError;
    if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
        cerr << "Error al ejecutar SQL: " << mensajeError << endl;
        sqlite3_free(mensajeError);
    } else {
        cout << "Operacion Exitosa." << endl;
    }
}

// Esta funcion abre la base de datos (SistemaBancario.db)
sqlite3* abrirBaseDatos(const char* nombre) {
    sqlite3* db;
    if (sqlite3_open(nombre, &db) != SQLITE_OK) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}

// Funcion para verificar si una cuenta existe en una tabla
bool cuentaExiste(sqlite3* db, const std::string& tabla, int id) {
    std::string sql = "SELECT COUNT(*) FROM " + tabla + " WHERE id = " + std::to_string(id) + ";";
    sqlite3_stmt* stmt;
    bool existe = false;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            existe = true;
        }
    } else {
        cerr << "Error al verificar la cuenta: " << sqlite3_errmsg(db) << endl;
    }
    sqlite3_finalize(stmt);
    return existe;
}

// Funcion para borrar una cuenta en una tabla (Cuenta_Colones o Cuenta_Dolares)
void borrarCuenta(sqlite3* db) {
    int id;
    string tabla;

    cout << "Ingrese el ID de la cuenta que desea borrar: ";
    cin >> id;
    cin.ignore();

    // Seleccion de la tabla
    cout << "Ingrese el tipo de moneda de la cuenta (Cuenta_Colones o Cuenta_Dolares): ";
    getline(cin, tabla);

    // Verificar si la tabla ingresada es valida
    if (tabla != "Cuenta_Colones" && tabla != "Cuenta_Dolares") {
        cerr << "Tabla invalida. Debe ser 'Cuenta_Colones' o 'Cuenta_Dolares'." << endl;
        return;
    }

    // Verificar si la cuenta existe
    if (!cuentaExiste(db, tabla, id)) {
        cout << "La cuenta con ID " << id << " no existe " << tabla << "." << endl;
        return;
    }

    // Borrar la cuenta
    std::string sql = "DELETE FROM " + tabla + " WHERE id = " + std::to_string(id) + ";";
    ejecutarSQL(db, sql.c_str());

    // Mostrar mensaje de confirmación
    cout << "Cuenta con ID " << id << " eliminada " << tabla << "." << endl;
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
