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
sqlite3* abrirBaseDatos(const char* nombre) {
    sqlite3* db;
    if (sqlite3_open(nombre, &db) != SQLITE_OK) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl; // Error en caso de que esta no se pueda abrir
        return nullptr;
    }
    return db;
}

// ----- Código de ConsultaDB -----
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
