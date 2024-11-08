#include "CrearCuenta.hpp"
#include "AbrirDB.hpp"
#include <openssl/evp.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Hash.hpp"

using namespace std;

// Función para ejecutar comandos SQL sin retorno

void ejecutarSQL(sqlite3* db, const char* sql) {
    char* mensajeError;
    if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
        cerr << "Error al ejecutar SQL: " << mensajeError << endl;
        sqlite3_free(mensajeError);
    } else {
        cout << "Operacion realizada correctamente." << endl;
    }
}

// Esta funcion lee la contrasena que este ingresando y la oculta (Unix y Windows trabajan con bibliotecas distintas)

string leerPassword() {
    string Password;
    char ch;

    cout << "Ingrese su Password (oculta): ";
    
#ifdef _WIN32
    // Windows
    while ((ch = _getch()) != '\r') { // '\r' representa Enter en Windows
        if (ch == '\b') { // Maneja backspace
            if (!Password.empty()) {
                cout << "\b \b"; // Borra el último carácter
                Password.pop_back();
            }
        } else {
            Password += ch;
            cout << '*'; // Muestra un asterisco en lugar del carácter
        }
    }
#else
    // Unix/Linux
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n') {
        if (ch == 127 || ch == '\b') { // Maneja backspace
            if (!Password.empty()) {
                cout << "\b \b";
                Password.pop_back();
            }
        } else {
            Password += ch;
            cout << '*';
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    cout << endl;
    return Password;
}

// Genera el id random con un prefijo (100 colones, 200 dolares)
int generarID(int prefijo) {
    return prefijo * 1000000 + rand() % 1000000;
}

// Genera el cvv el cual es de 3 digitos
int generarCVV() {
    return rand() % 900 + 100; // Genera un número entre 100 y 999
}

// Insersion a las tablas
void crearCuenta(sqlite3* db, const string& tabla, const string& nombre, const string& Password, int id, int cvv) {
    string hashedPassword = hashSHA256(Password);  // Hashear la contraseña

    string sql = "INSERT INTO " + tabla + " (nombre, id, salida_pais, cantidad_dinero, cvv, Password) VALUES ('" +
                      nombre + "', " + to_string(id) + ", 0, 0, " + to_string(cvv) + ", '" + hashedPassword + "');";

    ejecutarSQL(db, sql.c_str());
}

// Gestiona la creacion de las cuentas
void gestionarCreacionCuenta(sqlite3* db) {
    string nombre, Password;
    int opcion;

    cout << "Ingrese su nombre: ";
    getline(cin, nombre);

    // Leer la contraseña y hacer el hash llamando a leerPasword
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
    cin.ignore(); // Limpiar el buffer 

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

