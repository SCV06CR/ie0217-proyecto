#ifndef ABRIR_DB_HPP
#define ABRIR_DB_HPP
#include <sqlite3.h>

// Declaración de la función abrirBaseDatos
sqlite3* abrirBaseDatos(const char* nombre);

#endif // ABRIR_DB_HPP
