#ifndef BASE_DATOS_HPP
#define BASE_DATOS_HPP

#include <sqlite3.h>
#include <iostream>

using namespace std;

// Funcion que ejecuta consultas sin retorno
void ejecutarSQL(sqlite3* db, const char* sql);

// Función para abrir la base de datos
sqlite3* abrirBaseDatos(const char* nombre);

// Función para ejecutar una consulta y mostrar el resultado
void consultarTabla(sqlite3* db, const char* tabla);

// Crear las tablas si no existen
void crearTablas();


#endif // BASE_DATOS_HPP