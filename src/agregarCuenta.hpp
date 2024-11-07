#ifndef AGREGAR_CUENTA_HPP
#define AGREGAR_CUENTA_HPP

#include <iostream>
#include <sqlite3.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>
#include "baseDatos.hpp"


// Funcion para la generacion de un numero aleatorio en un rango (0 a 999999999 id, 100 a 999 cvv)
int generarNumeroAleatorio(int min, int max);

// Funcion para crear una cuenta en la tabla Cuenta_Colones
void crearCuentaColones(sqlite3* db);

// Funcion para crear una cuenta en la tabla Cuenta_Dolares
void crearCuentaDolares(sqlite3* db, string &nombreUser);

// Funcion para almacenar datos de login en una tabla
void loginCuenta(sqlite3* db, string &nombreUser, string &contrasenaUser);

void crearCuenta(int &option2, string &nombreUser);


#endif // AGREGAR_CUENTA_HPP