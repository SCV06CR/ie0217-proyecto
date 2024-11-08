#ifndef CONSULTAR_DB_HPP
#define CONSULTAR_DB_HPP

#include <sqlite3.h>
#include <vector>
#include <string>

// Declara la función consultarTabla
void consultarTabla(sqlite3* db, const char* tabla);

#endif // CONSULTAR_DB_HPP
