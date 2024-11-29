#ifndef BASEDATOS_HPP
#define BASEDATOS_HPP

#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;


sqlite3* abrirBaseDatos(const char* nombre);


void consultarTabla(sqlite3* db, const char* tabla);


string hashSHA256(const string& input);


double generarTipoCambio(double min, double max);

#endif // BASEDATOS_HPP
