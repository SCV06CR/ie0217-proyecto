#include "baseDatos.hpp"

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

// Función para crear las tablas de la base de datos
void crearTablas(sqlite3* db) {

    if (db) {
    
    // Habilitar claves foráneas
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);

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
            FOREIGN KEY (id) REFERENCES Cuenta_Colones(id) ON DELETE CASCADE
        );
    )";

    // Tabla de Préstamos
    const char* sqlPrestamosColones = R"(
        CREATE TABLE IF NOT EXISTS Prestamos_Colones (
            id_prestamo INTEGER PRIMARY KEY CHECK(id_prestamo BETWEEN 0 AND 999999) NOT NULL,
            id_cuenta INTEGER NOT NULL,
            intereses INTEGER NOT NULL,
            meses INTEGER NOT NULL,
            monto INTEGER NOT NULL,
            tipo_prestamo TEXT NOT NULL,
            cuotas_pagadas INTEGER NOT NULL CHECK(cuotas_pagadas >= 0),
            FOREIGN KEY (id_cuenta) REFERENCES Cuenta_Colones(id) ON DELETE CASCADE
        );
    )";

    const char* sqlPrestamosDolares = R"(
        CREATE TABLE IF NOT EXISTS Prestamos_Dolares (
            id_prestamo INTEGER PRIMARY KEY CHECK(id_prestamo BETWEEN 0 AND 999999) NOT NULL,
            id_cuenta INTEGER NOT NULL,
            intereses INTEGER NOT NULL,
            meses INTEGER NOT NULL,
            monto INTEGER NOT NULL,
            tipo_prestamo TEXT NOT NULL,
            cuotas_pagadas INTEGER NOT NULL CHECK(cuotas_pagadas >= 0),
            FOREIGN KEY (id_cuenta) REFERENCES Cuenta_Dolares(id) ON DELETE CASCADE
        );
    )";

    // Crear las tablas
    ejecutarSQL(db, sqlCuentaColones);
    ejecutarSQL(db, sqlCuentaDolares);
    ejecutarSQL(db, sqlMovimientos);
    ejecutarSQL(db, sqlPrestamosColones);
    ejecutarSQL(db, sqlPrestamosDolares);

    }
}
