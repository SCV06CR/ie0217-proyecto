#ifndef VERIFICACION_HPP
#define VERIFICACION_HPP
#include <sqlite3.h>
#include <string>
using namespace std;

// Función para verificar si existe una cuenta con un ID y contraseña dados
bool verificarCuenta(sqlite3* db, int id, const string& password);

#endif // VERIFICACION_HPP
