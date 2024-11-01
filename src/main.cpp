#include <iostream> 
#include "TipoCambio.hpp"

using namespace std; 

int main(){
    double TipoCambioMin = 520.0;
    double TipoCambioMax = 537.5;
    double TipoCambio = generarTipoCambio(TipoCambioMin, TipoCambioMax); 

    //Display simplemente estético para el sistema del banco.
    cout << "-----------------------------------------------------------------------------" << endl; 
    cout << "|                     Bienvenido al Sistema en ventanilla                   |" <<  endl;
    cout << "-----------------------------------------------------------------------------" << endl;  
    cout << "|                                      |                                    |" <<  endl;
    cout << "|                                      |                                    |" <<  endl;
    cout << "|        1. Ingresar al Sistema        |        2. Crear una Cuenta         |" <<  endl;
    cout << "|                                      |                                    |" <<  endl;
    cout << "|                                      |                                    |" <<  endl;
    //Parte de abajo del programa para hacer display del tipo de cambio. 
    cout << "-----------------------------------------------------------------------------" << endl;  
    cout << "|                                                                           |" <<  endl;
    cout << "|                                                                           |" <<  endl;
    cout << "|                               Tipo de cambio:                             |" <<  endl;
    cout << "|                               " << TipoCambio << "  CRC/USD                             |   " << endl;
    cout << "|                                                                           |" <<  endl;
    cout << "|                                                                           |" <<  endl;
    cout << "|---------------------------------------------------------------------------|" <<  endl;
}

