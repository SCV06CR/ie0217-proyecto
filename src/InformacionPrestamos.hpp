#ifndef INFORMACION_PRESTAMOS_HPP
#define INFORMACION_PRESTAMOS_HPP

#include<string> 
#include <iostream>
#include <sqlite3.h>
#include "AbrirDB.hpp"
#include "CrearCuenta.hpp"
#include "ConsultaDB.hpp"
#include "Verificacion.hpp"
#include <math.h>
#include <time.h>

using namespace std;

// Función para generar un ID con prefijo especificado
int generarPrestamoID(int prefijo);

// Menu para manejo de la gestion de prestamos
void showMenuSP(const int& idCuenta, sqlite3* db);

// Sub menu para la solicitud de un prestamo
void gestionarPrestamo(sqlite3* db, const int& idCuenta);

// Funcion que almacena los datos de un prestamo en las tablas correspondientes
void solicitarPrestamo(sqlite3* db, const string& tabla,const int& idCuenta, int& interes, int& monto, int& plazoPrestamo, const string& tipoPrestamo);

// Funcion para imprimir los datos de un prestamo dado su ID
void consultaPrestamo(sqlite3* db, int &idPrestamo, const string& tabla);

// Funcion para generar un reporte de un prestamo determinado
void reportePrestamos(sqlite3* db);

// Funcion para extraer un dato entero de la tabla de  prestamos
int extraerDatoEntero(sqlite3* db,string &strIDPrestamo, string tabla, string columna);

// Funcion para extraer un dato entero de la tabla de  prestamos
double extraerDatoDouble(sqlite3* db,string &strIDPrestamo, string tabla, string columna);

#endif // INFORMACION_PRESTAMOS_HPP