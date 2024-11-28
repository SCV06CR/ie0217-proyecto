#ifndef CREAR_CUENTA_HPP
#define CREAR_CUENTA_HPP
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

// Biblioteca de Windows conio.h, En caso de usar Unix utiliza termios.h y unistd.h

#ifdef _WIN32
    #include <conio.h> 
#else
    #include <termios.h> 
    #include <unistd.h>
#endif


using namespace std;


// Función para ocultar la entrada de la contraseña en la terminal
string leerPassword();

// Función para generar un ID con prefijo especificado
int generarID(int prefijo);

// Función para generar un CVV aleatorio de 3 dígitos
int generarCVV();

// Función para crear una cuenta en la tabla especificada
void crearCuenta(sqlite3* db, const string& tabla, const string& nombre, const string& Password, int id, int cvv);

// Función principal para gestionar la creación de cuentas
void gestionarCreacionCuenta(sqlite3* db);

#endif // CREAR_CUENTA_HPP
