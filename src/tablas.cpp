#include <iostream>
#include <sqlite3.h>
using namespace std;


// Función para abrir la base de datos
sqlite3* abrirBaseDatos(const char* nombre) {
    sqlite3* db;
    if (sqlite3_open(nombre, &db) != SQLITE_OK) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}

// Función para ejecutar una consulta y mostrar el resultado
void consultarTabla(sqlite3* db, const char* tabla) {
    string sql = "SELECT * FROM ";
    sql += tabla;

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        int colCount = sqlite3_column_count(stmt);

        // Mostrar los nombres de las columnas
        for (int i = 0; i < colCount; i++) {
            cout << sqlite3_column_name(stmt, i) << "\t";
        }
        cout << endl;

        // Ejecutar y mostrar cada fila
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < colCount; i++) {
                const unsigned char* val = sqlite3_column_text(stmt, i);
                cout << (val ? val : (const unsigned char*)"(NULL)") << "\t";
            }
            cout << endl;
        }
    } else {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

int main() {
    sqlite3* db = abrirBaseDatos("SistemaBancario.db");

    if (db) {
        // Habilitar claves foráneas
        sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);

        // Consultar la tabla 'Cuenta_Colones' para verificar su creación
        cout << "Tabla Cuenta_Colones:" << endl;
        consultarTabla(db, "Cuenta_Colones");

        sqlite3_close(db);
    }

    return 0;
}
