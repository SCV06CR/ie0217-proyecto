#include "CrearCuenta.hpp"
#include "AbrirDB.hpp"
#include <openssl/evp.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Hash.hpp"

using namespace std;

void ejecutarSQL(sqlite3* db, const char* sql) {
    char* mensajeError;
    if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
        cerr << "Error al ejecutar SQL: " << mensajeError << endl;
        sqlite3_free(mensajeError);
    } else {
        cout << "Operacion realizada correctamente." << endl;
    }
}

string leerPassword() {
    string Password;
    char ch;

    cout << "Ingrese una password para su cuenta (Esta aparece oculta y puede contener letras y numeros en un maximo de 12 caracteres): ";
    while ((ch = _getch()) != '\r') { // Captura caracteres hasta que presione "Enter"
        if (ch == '\b' && !Password.empty()) { // Permite borrar un carácter
            Password.pop_back();
            cout << "\b \b"; // Borra visualmente en consola
        } else if (ch != '\b') {
            Password.push_back(ch);
            cout << '*'; // Muestra un asterisco en lugar del carácter
        }
    }
    cout << endl;

    return Password;
}


int generarID(int prefijo) {
    return prefijo * 1000000 + rand() % 1000000;
}

int generarCVV() {
    return rand() % 900 + 100; // Genera un número entre 100 y 999
}

void crearCuenta(sqlite3* db, const string& tabla, const string& nombre, const string& Password, int id, int cvv) {
    string hashedPassword = hashSHA256(Password);  // Hashear la contraseña

    string sql = "INSERT INTO " + tabla + " (nombre, id, salida_pais, cantidad_dinero, cvv, Password) VALUES ('" +
                      nombre + "', " + to_string(id) + ", 0, 0, " + to_string(cvv) + ", '" + hashedPassword + "');";

    ejecutarSQL(db, sql.c_str());
}

void gestionarCreacionCuenta(sqlite3* db) {
    string nombre, Password;
    int opcion;

    cout << "Ingrese su nombre: ";
    getline(cin, nombre);

    // Leer la contraseña y hacer el hash
    Password = leerPassword();
    while (Password.length() > 12) {
        cerr << "La contraseña es demasiado larga\n";
        Password = leerPassword();
    }

    // Menú de opciones
    cout << "Seleccione una opcion:\n";
    cout << "1. Crear solo cuenta en Colones\n";
    cout << "2. Crear solo cuenta en Dolares\n";
    cout << "3. Crear ambas cuentas\n";
    cin >> opcion;
    cin.ignore(); // Limpiar el buffer de entrada

    // Variables para ID y CVV
    int idColones, idDolares, cvvColones, cvvDolares;

    switch (opcion) {
        case 1:
            // Crear solo cuenta en Colones
            idColones = generarID(100);
            cvvColones = generarCVV();
            crearCuenta(db, "Cuenta_Colones", nombre, Password, idColones, cvvColones);
            break;

        case 2:
            // Crear solo cuenta en Dólares
            idDolares = generarID(200);
            cvvDolares = generarCVV();
            crearCuenta(db, "Cuenta_Dolares", nombre, Password, idDolares, cvvDolares);
            break;

        case 3:
            // Crear ambas cuentas
            idColones = generarID(100);
            idDolares = generarID(200);
            cvvColones = generarCVV();
            cvvDolares = generarCVV();

            crearCuenta(db, "Cuenta_Colones", nombre, Password, idColones, cvvColones);
            crearCuenta(db, "Cuenta_Dolares", nombre, Password, idDolares, cvvDolares);
            break;

        default:
            cerr << "Opcion invalida.\n";
            return;
    }

    cout << "Cuenta(s) creada(s) exitosamente.\n";
}

