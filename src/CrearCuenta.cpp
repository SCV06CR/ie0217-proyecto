#include "CrearCuenta.hpp"
#include "AbrirDB.hpp"
#include <openssl/evp.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Hash.hpp"
#include "baseDatos.hpp"

using namespace std;

// Esta funcion lee la contrasena que este ingresando y la oculta (Unix y Windows trabajan con bibliotecas distintas)
/**
 * @brief Lee y oculta la contraseña ingresada por el usuario.
 * 
 * Esta función captura la contraseña ingresada por el usuario de manera segura, mostrando asteriscos en lugar de los 
 * caracteres reales, y ocultando la contraseña en la terminal. Se maneja de forma diferente en sistemas Windows y Unix.
 * 
 * @return string La contraseña ingresada por el usuario.
 */
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
/**
 * @brief Genera un ID aleatorio para la cuenta bancaria.
 * 
 * El ID se genera combinando un prefijo con un número aleatorio de 6 dígitos, permitiendo la creación de IDs únicos 
 * para diferentes tipos de cuenta (Colones, Dólares, etc.).
 * 
 * @param prefijo El prefijo que se usará para diferenciar los tipos de cuenta (por ejemplo, 100 para Colones y 200 para Dólares).
 * @return int El ID generado para la cuenta.
 */
int generarID(int prefijo) {
    return prefijo * 1000000 + rand() % 1000000;
}

// Genera el cvv el cual es de 3 digitos
/**
 * @brief Genera un número CVV de 3 dígitos.
 * 
 * El CVV es un número aleatorio entre 100 y 999, utilizado para la verificación de la tarjeta de crédito o débito.
 * 
 * @return int El CVV generado.
 */
int generarCVV() {
    return rand() % 900 + 100; // Genera un número entre 100 y 999
}

// Insersion a las tablas
/**
 * @brief Inserta una nueva cuenta en la base de datos.
 * 
 * Esta función inserta una nueva cuenta en la tabla correspondiente, ya sea en la cuenta de Colones o Dólares, 
 * junto con la información del nombre del usuario, ID, CVV y una contraseña hasheada.
 * 
 * @param db Puntero a la base de datos SQLite.
 * @param tabla Nombre de la tabla en la que se insertará la cuenta (puede ser "Cuenta_Colones" o "Cuenta_Dolares").
 * @param nombre Nombre del titular de la cuenta.
 * @param Password Contraseña del titular de la cuenta.
 * @param id ID de la cuenta.
 * @param cvv CVV de la cuenta.
 */
void crearCuenta(sqlite3* db, const string& tabla, const string& nombre, const string& Password, int id, int cvv) {
    string hashedPassword = hashSHA256(Password);  // Hashear la contraseña

    string sql = "INSERT INTO " + tabla + " (nombre, id, salida_pais, cantidad_dinero, cvv, Password) VALUES ('" +
                      nombre + "', " + to_string(id) + ", 0, 0, " + to_string(cvv) + ", '" + hashedPassword + "');";

    ejecutarSQL(db, sql.c_str());
}

// Gestiona la creacion de las cuentas
/**
 * @brief Gestiona la creación de cuentas bancarias.
 * 
 * Esta función guía al usuario a través del proceso de creación de una cuenta bancaria, permitiéndole elegir entre 
 * crear una cuenta en Colones, en Dólares o ambas. Se valida que la contraseña no sea demasiado larga y se generan 
 * los IDs y CVV correspondientes para cada cuenta.
 * 
 * @param db Puntero a la base de datos SQLite.
 */
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

