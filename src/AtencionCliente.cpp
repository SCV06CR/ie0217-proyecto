#include"AtencionCliente.hpp"


/*
************************************************
*Falta hacer la implementacion de las funciones*
************************************************
*/

using namespace std; 

// Esta funcion recibe como argumentos el id de la cuenta a la que se le quiere hacer un deposito
// Recibe como argumento el monto a depositar y recibe la base de datos a la cual se le va a ingresar el dinero. 
void realizadDeposito(const string& id, const double& monto, sqlite3* db){}

// Esta funcion recibe como argumentos el id de la cuenta a la que se le quiere hacer un deposito
// Recibe como argumento el monto a depositar
// Recibe la base de datos donde existe la cuenta a la cual se le va a ingresar el dinero.
void realizadRetiro(const string& id, const double& monto, sqlite3* db){}


// El primer const id es para la cuenta de origen 
// El segundo const id es para la cuenta destino 
// se recibe el monto a transferir 
// Recibe la base de datos donde existe la cuenta a la cual se le va a ingresar el dinero.
void realizadTransferencia(const string& idOrigen, const string& idDestino, const double& monto,sqlite3* db){}

// El primer const id es del préstamo
// El segundo const id es de la cuenta
// se recibe el monto para abonar
// Recibe la base de datos donde existe la cuenta y el préstamo.
void realizarAbono(const string& idPrestamo, const string& idCuenta, const double& monto, sqlite3* db){}


// El primer const id es del préstamo
// El segundo const id es de la cuenta
// Recibe la base de datos donde existe la cuenta y el préstamo.
void pagarCuota(const string& idPrestamo, const string& idCuenta, sqlite3* db){}

// Recibe el id de la cuenta la cual se quiere registrar una salida del pais 
// Recibe la base de datos donde existe la cuenta
void registrarSalidaPais(const string& idCuenta, sqlite3* db){}

// Recibe el id de la cuenta la cual se quiere consultar PIN del cajero 
// Recibe la base de datos donde existe la cuenta
void consultarPin(const string& idCuenta, sqlite3* db){}


// Recibe el id de la cuenta la cual se quiere consultar el CVV
// Recibe la base de datos donde existe la cuenta
void consultarCVV(const string& idCuenta, sqlite3* db){}


// Recibe el id de la cuenta que se quiere realizar la impresión del estado de cuenta
// Recibe una base de datos 
void imprimirEstadoCuenta(const string& idCuenta, sqlite3* db){}



