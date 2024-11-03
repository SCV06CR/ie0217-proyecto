#include <iostream>
#include <sqlite3.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>
using namespace std;

// Funcion que ejecuta consultas sin retorno
void ejecutarSQL(sqlite3* db, const char* sql) {
    char* mensajeError;
    if (sqlite3_exec(db, sql, 0, 0, &mensajeError) != SQLITE_OK) {
        cerr << "Error al ejecutar SQL: " << mensajeError << endl;
        sqlite3_free(mensajeError);
    } else {
        cout << "Operacion realizada con exito " << endl;
    }
}

// Esta funcion abre la base de datos (SistemaBancario.db)
sqlite3* abrirBaseDatos(const char* nombre) {
    sqlite3* db;
    if (sqlite3_open(nombre, &db) != SQLITE_OK) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}

// Funcion para consultar y mostrar el contenido de una tabla (Cuenta_Dolares)
void consultarTabla(sqlite3* db, const char* tabla) {
    string sql = "SELECT * FROM ";
    sql += tabla;

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        int colCount = sqlite3_column_count(stmt);

        // Mostrar los nombres de las columnas
        for (int i = 0; i < colCount; i++) {
            cout << sqlite3_column_name(stmt, i) << "\t";
        }
        cout << endl;

        // Mostrar cada fila
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < colCount; i++) {
                const unsigned char* val = sqlite3_column_text(stmt, i);
                cout << (val ? val : (const unsigned char*)"(NULL)") << "\t";
            }
            cout << endl;
        }
    } else {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

// Funcion para la generacion de un numero aleatorio en un rango (0 a 999999999 id, 100 a 999 cvv)
int generarNumeroAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Funcion para crear una cuenta en la tabla Cuenta_Dolares
void crearCuentaDolares(sqlite3* db) {
    // Generar un ID y CVV aleatorios
    int id = generarNumeroAleatorio(0, 999999999);
    int cvv = generarNumeroAleatorio(100, 999);

    // Solicita el nombre del usuario y validar que solo contenga letras y/o espacios
    string nombre;
    bool nombreValido = false;
    do {
        cout << "Ingrese su nombre: ";
        getline(cin, nombre);

        nombreValido = !nombre.empty();
        for (char c : nombre) {
            if (!isalpha(c) && c != ' ') {
                nombreValido = false;
                break;
            }
        }

        if (!nombreValido) {
            cout << "Error: El nombre solo debe contener letras y espacios." << endl;
        }
    } while (!nombreValido);

    // Valores predeterminados para salida_pais y cantidad_dinero (false 0, y cantidad de dinero en la cuenta = 0 ya que esta vacia
    // hasta que se realice un deposito)
    bool salida_pais = false;
    int cantidad_dinero = 0;

    // Crear e insertar el registro en la base de datos
    std::string sql = "INSERT INTO Cuenta_Dolares (id, nombre, salida_pais, cantidad_dinero, cvv) VALUES (" +
                      std::to_string(id) + ", '" + nombre + "', " +
                      std::to_string(salida_pais) + ", " +
                      std::to_string(cantidad_dinero) + ", " +
                      std::to_string(cvv) + ");";

    ejecutarSQL(db, sql.c_str());

    // Mostrar el contenido de la tabla para verificar la insercion
    cout << "\nCuenta_Dolares ha sido actualizada con los siguientes datos:" << endl;
    consultarTabla(db, "Cuenta_Dolares");
}

int main() {
    // Semilla para números aleatorios
    srand(static_cast<unsigned>(time(0)));

    // Abrir la base de datos
    sqlite3* db = abrirBaseDatos("SistemaBancario.db");

    if (db) {
        // Crear cuenta en dolares
        crearCuentaDolares(db);

        // Cerrar la base de datos
        sqlite3_close(db);
    }

    return 0;
}
