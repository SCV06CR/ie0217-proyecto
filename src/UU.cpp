#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sqlite3.h>
#include <vector>
#include <algorithm>

using namespace std;

// ----- Código de AbrirDB -----
/**
 * @brief Abre una base de datos SQLite.
 * 
 * Esta función intenta abrir la base de datos especificada por el nombre. Si no puede abrirla, devuelve un puntero nulo
 * y muestra un mensaje de error. Si la base de datos se abre correctamente, devuelve el puntero de la base de datos.
 * 
 * @param nombre El nombre de la base de datos a abrir.
 * @return sqlite3* Puntero a la base de datos abierta o nullptr si ocurre un error.
 * 
 * @note Esta función utiliza SQLite3 para abrir la base de datos.
 */
sqlite3* abrirBaseDatos(const char* nombre) {
    sqlite3* db;
    if (sqlite3_open(nombre, &db) != SQLITE_OK) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl; // Error en caso de que esta no se pueda abrir
        return nullptr;
    }
    return db;
}

// ----- Código de ConsultaDB -----
/**
 * @brief Consulta y muestra el contenido de una tabla en la base de datos SQLite.
 * 
 * Esta función ejecuta una consulta SQL para seleccionar todos los registros de la tabla especificada. Los resultados
 * se imprimen en la consola con un formato tabular, donde se alinean las columnas de manera adecuada.
 * 
 * @param db Puntero a la base de datos SQLite que contiene la tabla.
 * @param tabla Nombre de la tabla que se desea consultar.
 * 
 * @note Si la consulta o la preparación fallan, se muestra un mensaje de error.
 */
void consultarTabla(sqlite3* db, const char* tabla) {
    string sql = "SELECT * FROM ";
    sql += tabla;

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        int colCount = sqlite3_column_count(stmt);

        // Obtener los nombres de las columnas y calcular los anchos para alineación
        vector<int> colWidths(colCount, 12); // Ajusta el ancho de cada columna si es necesario
        for (int i = 0; i < colCount; i++) {
            string colName = reinterpret_cast<const char*>(sqlite3_column_name(stmt, i));
            colWidths[i] = max(colWidths[i], static_cast<int>(colName.length()));
            cout << left << setw(colWidths[i]) << colName << " | ";
        }
        cout << endl;

        // Línea divisoria
        for (int i = 0; i < colCount; i++) {
            cout << string(colWidths[i], '-') << "-+-";
        }
        cout << endl;

        // Ejecutar y mostrar cada fila con el mismo ancho de columna
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < colCount; i++) {
                const unsigned char* val = sqlite3_column_text(stmt, i);
                cout << left << setw(colWidths[i]) << (val ? reinterpret_cast<const char*>(val) : "(NULL)") << " | ";
            }
            cout << endl;
        }
    } else {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

// ----- Código de Hash -----
/**
 * @brief Calcula el hash SHA-256 de una cadena de texto.
 * 
 * Esta función toma una cadena de entrada y calcula su hash utilizando el algoritmo SHA-256. El resultado se devuelve
 * como una cadena de texto hexadecimal que representa el valor del hash.
 * 
 * @param input La cadena de texto a la que se le calculará el hash SHA-256.
 * @return string El hash SHA-256 en formato hexadecimal.
 * 
 * @note Utiliza la biblioteca OpenSSL para calcular el hash.
 */
string hashSHA256(const string& input) {
    unsigned char hash[EVP_MAX_MD_SIZE]; // Arreglo para almacenar el hash resultante
    unsigned int length = 0;

    // Crea un nuevo contexto de hashing
    EVP_MD_CTX* ctx = EVP_MD_CTX_new(); 
    if (ctx == nullptr) { // Verifica si el contexto fue creado exitosamente
        cerr << "Error al crear el contexto para SHA-256." << endl;
        return "";
    }

    // Inicializa el contexto para utilizar SHA-256 como algoritmo de hash
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        cerr << "Error al inicializar el algoritmo SHA-256." << endl;
        EVP_MD_CTX_free(ctx);
        return "";
    }

    // Actualiza el hash con los datos de entrada
    if (EVP_DigestUpdate(ctx, input.c_str(), input.length()) != 1) {
        cerr << "Error al actualizar el hash." << endl;
        EVP_MD_CTX_free(ctx);
        return "";
    }

    // Finaliza el cálculo del hash y almacena el resultado en el arreglo
    if (EVP_DigestFinal_ex(ctx, hash, &length) != 1) {
        cerr << "Error al finalizar el hash." << endl;
        EVP_MD_CTX_free(ctx);
        return "";
    }

    // Libera el contexto después de calcular el hash
    EVP_MD_CTX_free(ctx);

    // Convierte el hash binario a una representación en hexadecimal
    stringstream ss;
    for (unsigned int i = 0; i < length; ++i) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str(); // Devuelve el hash en formato hexadecimal como un string
}

// ----- Código de TipoCambio -----
double generarTipoCambio(double min, double max) {
    // Generar un número simple en el rango de mínimo y máximo
    double randomFact = (1.0 * (max - min)) / 10; // Simular una varianza 
    return min + randomFact;  
}
