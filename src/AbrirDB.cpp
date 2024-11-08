#include "AbrirDB.hpp"
#include <iostream>
using namespace std;

sqlite3* abrirBaseDatos(const char* nombre) {
    sqlite3* db;
    if (sqlite3_open(nombre, &db) != SQLITE_OK) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}
