#ifndef INFORMACION_PRESTAMOS_HPP
#define INFORMACION_PRESTAMOS_HPP

#include<string> 
#include <iostream>
#include <sqlite3.h>

using namespace std;

void solicitarPrestamoPersonal(const string& idCuenta, sqlite3* db);

void solicitarPrestamoPrendario(const string& idCuenta, sqlite3* db);

void solicitarPrestamoHipotecario(const string& idCuenta, sqlite3* db);

#endif // INFORMACION_PRESTAMOS_HPP