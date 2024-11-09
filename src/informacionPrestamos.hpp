#ifndef INFORMACION_PRESTAMOS_HPP
#define INFORMACION_PRESTAMOS_HPP

#include<string> 
#include <iostream>
#include <sqlite3.h>
#include "AbrirDB.hpp"
#include "CrearCuenta.hpp"

using namespace std;

int generarPrestamoID(int prefijo);

void gestionarPrestamo(sqlite3* db, const int& idCuenta);

void solicitarPrestamo(sqlite3* db, const string& tabla,const int& idCuenta, int& interes, int& monto, int& plazoPrestamo, const string& tipoPrestamo);


#endif // INFORMACION_PRESTAMOS_HPP