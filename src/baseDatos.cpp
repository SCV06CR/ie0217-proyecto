#include "baseDatos.hpp"

/**
 * @brief Ejecuta un comando SQL sin retorno.
 * 
 * Esta función se utiliza para ejecutar comandos SQL que no requieren un valor de retorno, como la 
 * creación de tablas o la inserción de datos. En caso de error, muestra el mensaje de error.
 * 
 * @param db Puntero a la base de datos SQLite donde se ejecutará el comando.
 * @param sql Cadena de texto con el comando SQL a ejecutar.
 */
void ejecutarSQL(sqlite3* db, const char* sql) {
    char* mensajeError;
    if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
        cerr << "Error al ejecutar SQL: " << mensajeError << endl;
        sqlite3_free(mensajeError);
    } else {
        cout << "Operacion realizada correctamente." << endl;
    }
}

/**
 * @brief Crea las tablas necesarias en la base de datos.
 * 
 * Esta función crea las tablas para gestionar cuentas, movimientos y préstamos en la base de datos. 
 * Se incluyen tablas para cuentas en colones y dólares, movimientos de cuentas, y préstamos, 
 * tanto en colones como en dólares. Además, habilita las claves foráneas para mantener la integridad referencial.
 * 
 * @param db Puntero a la base de datos SQLite donde se crearán las tablas.
 */
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

    // Tabla de Movimientos de cuentas en colones
    // El id INTEGER puede ser cualquiera de los dos tipos de id de colones o dólares. 
    const char* sqlMovimientosColones = R"(
        CREATE TABLE IF NOT EXISTS Movimientos_Colones (
            id_cuenta INTEGER NOT NULL, -- Clave foránea a la tabla de cuentas en colones
            detalle TEXT NOT NULL, -- Descripción de lo que se hizo
            fecha TEXT DEFAULT CURRENT_TIMESTAMP NOT NULL, -- Fecha del movimiento
            FOREIGN KEY (id_cuenta) REFERENCES Cuenta_Colones(id) ON DELETE CASCADE
        );
    )";
   // Tabla de Movimientos de cuentas en dolares
    // El id INTEGER puede ser cualquiera de los dos tipos de id de colones o dólares. 
    const char* sqlMovimientosDolares = R"(
        CREATE TABLE IF NOT EXISTS Movimientos_Dolares (
            id_cuenta INTEGER NOT NULL, -- Clave foránea a la tabla de cuentas en dólares
            detalle TEXT NOT NULL, -- Descripción de lo que se hizo
            fecha TEXT DEFAULT CURRENT_TIMESTAMP NOT NULL, -- Fecha del movimiento
            FOREIGN KEY (id_cuenta) REFERENCES Cuenta_Dolares(id) ON DELETE CASCADE
        );
    )";


    // Tabla de Préstamos
    const char* sqlPrestamosColones = R"(
        CREATE TABLE IF NOT EXISTS Prestamos_Colones (
            id_prestamo INTEGER PRIMARY KEY CHECK(id_prestamo BETWEEN 0 AND 999999999) NOT NULL,
            id_cuenta INTEGER NOT NULL,
            intereses REAL NOT NULL,
            meses INTEGER NOT NULL,
            monto REAL NOT NULL,
            intereses_abonados REAL NOT NULL,
            saldo_restante REAL NOT NULL CHECK(saldo_restante >= 0),
            tipo_prestamo TEXT NOT NULL,
            monto_por_cuota FLOAT NOT NULL CHECK(monto_por_cuota >= 0),
            cuotas_pagadas INTEGER NOT NULL CHECK(cuotas_pagadas >= 0),
            FOREIGN KEY (id_cuenta) REFERENCES Cuenta_Colones(id) ON DELETE CASCADE
        );
    )";

    const char* sqlPrestamosDolares = R"(
        CREATE TABLE IF NOT EXISTS Prestamos_Dolares (
            id_prestamo INTEGER PRIMARY KEY CHECK(id_prestamo BETWEEN 0 AND 999999999) NOT NULL,
            id_cuenta INTEGER NOT NULL,
            intereses REAL NOT NULL,
            meses INTEGER NOT NULL,
            monto REAL NOT NULL,
            intereses_abonados REAL NOT NULL,
            saldo_restante REAL NOT NULL CHECK(monto_por_cuota >= 0),
            tipo_prestamo TEXT NOT NULL,
            monto_por_cuota REAL NOT NULL CHECK(monto_por_cuota >= 0),
            cuotas_pagadas INTEGER NOT NULL CHECK(cuotas_pagadas >= 0),
            FOREIGN KEY (id_cuenta) REFERENCES Cuenta_Dolares(id) ON DELETE CASCADE
        );
    )";

    // Crear las tablas
    ejecutarSQL(db, sqlCuentaColones);
    ejecutarSQL(db, sqlCuentaDolares);
    ejecutarSQL(db, sqlMovimientosColones);
    ejecutarSQL(db, sqlMovimientosDolares);
    ejecutarSQL(db, sqlPrestamosColones);
    ejecutarSQL(db, sqlPrestamosDolares);

    }
}
