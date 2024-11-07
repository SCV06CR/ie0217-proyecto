#include "agregarCuenta.hpp"

// Funcion para la generacion de un numero aleatorio en un rango (0 a 999999999 id, 100 a 999 cvv)
int generarNumeroAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Funcion para crear una cuenta en la tabla Cuenta_Colones
void crearCuentaColones(sqlite3* db, string &nombreUser) {
    // Generar un ID y CVV aleatorios
    int id = generarNumeroAleatorio(0, 999999999);
    int cvv = generarNumeroAleatorio(100, 999); // 3 dígitos

    // Valores predeterminados para salida_pais y cantidad_dinero (false 0, y cantidad de dinero en la cuenta = 0 ya que esta vacia
    // hasta que se realice un deposito)
    bool salida_pais = false;
    int cantidad_dinero = 0;

    // Crear e insertar el registro en la base de datos
    std::string sql = "INSERT INTO Cuenta_Colones (id, nombre, salida_pais, cantidad_dinero, cvv) VALUES (" +
                      std::to_string(id) + ", '" + nombreUser + "', " +
                      std::to_string(salida_pais) + ", " +
                      std::to_string(cantidad_dinero) + ", " +
                      std::to_string(cvv) + ");";

    ejecutarSQL(db, sql.c_str());

    // Mostrar el contenido de la tabla para verificar la insercion
    cout << "\nCuenta_Colones ha sido actualizada con los siguientes datos:" << endl;
    consultarTabla(db, "Cuenta_Colones");
}

// Funcion para crear una cuenta en la tabla Cuenta_Dolares
void crearCuentaDolares(sqlite3* db, string &nombreUser) {
    // Generar un ID y CVV aleatorios
    int id = generarNumeroAleatorio(0, 999999999);
    int cvv = generarNumeroAleatorio(100, 999);

    // Valores predeterminados para salida_pais y cantidad_dinero (false 0, y cantidad de dinero en la cuenta = 0 ya que esta vacia
    // hasta que se realice un deposito)
    bool salida_pais = false;
    int cantidad_dinero = 0;

    // Crear e insertar el registro en la base de datos
    std::string sql = "INSERT INTO Cuenta_Dolares (id, nombre, salida_pais, cantidad_dinero, cvv) VALUES (" +
                      std::to_string(id) + ", '" + nombreUser + "', " +
                      std::to_string(salida_pais) + ", " +
                      std::to_string(cantidad_dinero) + ", " +
                      std::to_string(cvv) + ");";

    ejecutarSQL(db, sql.c_str());

    // Mostrar el contenido de la tabla para verificar la insercion
    cout << "\nCuenta_Dolares ha sido actualizada con los siguientes datos:" << endl;
    consultarTabla(db, "Cuenta_Dolares");
}

// Funcion para almacenar datos de login en una tabla
void loginCuenta(sqlite3* db, string &nombreUser, string &contrasenaUser) {


}

void crearCuenta(int &option2, string &nombreUser) {
    // Semilla para números aleatorios
    srand(static_cast<unsigned>(time(0)));

    // Abrir la base de datos
    sqlite3* db = abrirBaseDatos("SistemaBancario.db");

    if (db) {
        switch(option2) {
            case 1:
                // Crear cuenta en colones
                crearCuentaColones(db, nombreUser);
                break;
            
            case 2:
                // Crear cuenta en dolares
                crearCuentaDolares(db, nombreUser);
                break;

            case 3:
                // Crear cuenta en colones
                crearCuentaColones(db, nombreUser);
                // Crear cuenta en dolares
                crearCuentaDolares(db, nombreUser);
                break;

            default: 
                cout << "\nOpción inválida... \n\nFavor ingrese una opción correcta.\n\n";
                break;

        }

        // Cerrar la base de datos
        sqlite3_close(db);
    }
}