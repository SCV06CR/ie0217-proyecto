#include "Hash.hpp"
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

string hashSHA256(const string& input) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (ctx == nullptr) {
        cerr << "Error al crear el contexto para SHA-256." << endl;
        return "";
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        cerr << "Error al inicializar el algoritmo SHA-256." << endl;
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (EVP_DigestUpdate(ctx, input.c_str(), input.length()) != 1) {
        cerr << "Error al actualizar el hash." << endl;
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (EVP_DigestFinal_ex(ctx, hash, &length) != 1) {
        cerr << "Error al finalizar el hash." << endl;
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    stringstream ss;
    for (unsigned int i = 0; i < length; ++i) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}
