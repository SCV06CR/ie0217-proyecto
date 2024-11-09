#include "Hash.hpp"
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

// SHA-256 es un algoritmo de hash criptográfico que toma una entrada de texto en este caso
// y la convierte en un valor de salida fijo de 256 bits (32 bytes).

// Función para generar un hash SHA-256 a partir de una cadena de entrada, texto este caso
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
