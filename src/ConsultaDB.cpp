#include "ConsultaDB.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Funcion que realiza consultas a las tablas 
void consultarTabla(sqlite3* db, const char* tabla) {
    string sql = "SELECT * FROM ";
    sql += tabla;

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        int colCount = sqlite3_column_count(stmt);

        // Obtener los nombres de las columnas y calcular los anchos para alineación
        vector<int> colWidths(colCount, 12); // Ajusta el ancho de cada columna si es necesario
        for (int i = 0; i < colCount; i++) {
            string colName = reinterpret_cast<const char*>(sqlite3_column_name(stmt, i));
            colWidths[i] = max(colWidths[i], static_cast<int>(colName.length()));
            cout << left << setw(colWidths[i]) << colName << " | ";
        }
        cout << endl;

        // Línea divisoria
        for (int i = 0; i < colCount; i++) {
            cout << string(colWidths[i], '-') << "-+-";
        }
        cout << endl;

        // Ejecutar y mostrar cada fila con el mismo ancho de columna
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < colCount; i++) {
                const unsigned char* val = sqlite3_column_text(stmt, i);
                cout << left << setw(colWidths[i]) << (val ? reinterpret_cast<const char*>(val) : "(NULL)") << " | ";
            }
            cout << endl;
        }
    } else {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}
