#ifndef ATENCIONCLIENTE_HPP
#define ATENCIONCLIENTE_HPP

#include <string>
#include <iostream>
#include <sqlite3.h>

using namespace std; 

void realizadDeposito(const int& id, sqlite3* db);
void realizadRetiro(const int& id, sqlite3* db);
void realizadTransferencia(const int& idOrigen, const int& idDestino, const double& TipoCambio, sqlite3* db);
void realizarAbono(const int& idPrestamo, const int& idCuenta, sqlite3* db);
void pagarCuota(const int& idPrestamo, const int& idCuenta, sqlite3* db);
void registrarSalidaPais(const int& idCuenta, sqlite3* db);
void consultarPin(const int& idCuenta, sqlite3* db);
void consultarCVV(const int& idCuenta, sqlite3* db);
void imprimirEstadoCuenta(const int& idCuenta, sqlite3* db); 
string showMenuAC(const int& idCuenta, const double& TipoCambio, sqlite3* db); 

#endif // ATENCIONCLIENTE_HPP
