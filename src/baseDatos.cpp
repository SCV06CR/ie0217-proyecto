#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std;

// Función para ejecutar comandos SQL sin retorno
void ejecutarSQL(sqlite3* db, const char* sql) {
    char* mensajeError;
    if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
        cerr << "Error al ejecutar SQL: " << mensajeError << endl;
        sqlite3_free(mensajeError);
    } else {
        cout << "Operacion realizada correctamente." << endl;
    }
}

// Función para abrir la base de datos
sqlite3* abrirBaseDatos(const char* nombre) {
    sqlite3* db;
    if (sqlite3_open(nombre, &db) != SQLITE_OK) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}

// Función para crear las tablas de la base de datos
void crearTablas(sqlite3* db) {
    // Tabla de Cuenta Colones
    const char* sqlCuentaColones = R"(
        CREATE TABLE IF NOT EXISTS Cuenta_Colones (
            id INTEGER PRIMARY KEY CHECK(id BETWEEN 0 AND 999999999) NOT NULL,
            nombre TEXT NOT NULL,
            salida_pais BOOLEAN NOT NULL CHECK(salida_pais IN (0, 1)),
            cantidad_dinero INTEGER NOT NULL CHECK(cantidad_dinero >= 0) DEFAULT 0,
            cvv INTEGER NOT NULL CHECK(cvv BETWEEN 100 AND 999),
            password TEXT NOT NULL
        );
    )";

    // Tabla de Cuenta Dólares
    const char* sqlCuentaDolares = R"(
        CREATE TABLE IF NOT EXISTS Cuenta_Dolares (
            id INTEGER PRIMARY KEY CHECK(id BETWEEN 0 AND 999999999) NOT NULL,
            nombre TEXT NOT NULL,
            salida_pais BOOLEAN NOT NULL CHECK(salida_pais IN (0, 1)),
            cantidad_dinero INTEGER NOT NULL CHECK(cantidad_dinero >= 0) DEFAULT 0,
            cvv INTEGER NOT NULL CHECK(cvv BETWEEN 100 AND 999),
            password TEXT NOT NULL
        );
    )";

    // Tabla de Movimientos
    const char* sqlMovimientos = R"(
        CREATE TABLE IF NOT EXISTS Movimientos (
            id INTEGER NOT NULL,
            tipo_movimiento TEXT CHECK(tipo_movimiento IN ('Transferencia', 'Deposito', 'Retiro')),
            monto INTEGER NOT NULL CHECK(monto > 0),
            fecha TEXT DEFAULT CURRENT_TIMESTAMP,
            password TEXT NOT NULL,
            FOREIGN KEY (id) REFERENCES Cuenta_Colones(id) ON DELETE CASCADE
        );
    )";

    // Tabla de Préstamos
    const char* sqlPrestamos = R"(
        CREATE TABLE IF NOT EXISTS Prestamos (
            id_prestamo INTEGER PRIMARY KEY CHECK(id_prestamo BETWEEN 0 AND 999999) NOT NULL,
            id_cuenta INTEGER NOT NULL,
            intereses INTEGER NOT NULL,
            meses INTEGER NOT NULL,
            monto INTEGER NOT NULL,
            cuotas_pagadas INTEGER NOT NULL CHECK(cuotas_pagadas >= 0),
            password TEXT NOT NULL,
            FOREIGN KEY (id_cuenta) REFERENCES Cuenta_Colones(id) ON DELETE CASCADE
        );
    )";

    // Crear las tablas
    ejecutarSQL(db, sqlCuentaColones);
    ejecutarSQL(db, sqlCuentaDolares);
    ejecutarSQL(db, sqlMovimientos);
    ejecutarSQL(db, sqlPrestamos);
}

// Función para modificar las tablas y agregar la columna "Password" si no existe (para tablas creadas previamente)
void modificarTablas(sqlite3* db) {
    const char* sqlCuentaColones = "ALTER TABLE Cuenta_Colones ADD COLUMN password TEXT;";
    const char* sqlCuentaDolares = "ALTER TABLE Cuenta_Dolares ADD COLUMN password TEXT;";
    const char* sqlPrestamos = "ALTER TABLE Prestamos ADD COLUMN password TEXT;";
    const char* sqlMovimientos = "ALTER TABLE Movimientos ADD COLUMN password TEXT;";

    ejecutarSQL(db, sqlCuentaColones);
    ejecutarSQL(db, sqlCuentaDolares);
    ejecutarSQL(db, sqlPrestamos);
    ejecutarSQL(db, sqlMovimientos);
}

// Función de hash para simular la encriptación de la contraseña (simulación)
string hashContraseña(const string& contraseña) {
    return "hashed_" + contraseña;  // Esta función debería reemplazarse con un hash real en producción
}

// Función para insertar una contraseña encriptada en la tabla especificada
void insertarContraseña(sqlite3* db, const string& tabla, int id, const string& contraseña) {
    string hashedContraseña = hashContraseña(contraseña); // Encriptamos la contraseña
    string sql = "UPDATE " + tabla + " SET password = '" + hashedContraseña + "' WHERE id = " + to_string(id) + ";";
    ejecutarSQL(db, sql.c_str());
}

int main() {
    // Abrir la base de datos
    sqlite3* db = abrirBaseDatos("SistemaBancario.db");

    if (db) {
        // Habilitar claves foráneas
        ejecutarSQL(db, "PRAGMA foreign_keys = ON;");
        
        // Crear las tablas
        crearTablas(db);

        // Modificar las tablas para agregar la columna "password" en caso de que ya existan
        modificarTablas(db);

        // Ejemplo de inserción de una contraseña encriptada para una cuenta en Cuenta_Colones
        int idCuenta = 1;  // ID de la cuenta para el ejemplo
        string contraseña = "mi_contraseña_secreta";
        insertarContraseña(db, "Cuenta_Colones", idCuenta, contraseña);

        // Cerrar la base de datos
        sqlite3_close(db);
    }

    return 0;
}
