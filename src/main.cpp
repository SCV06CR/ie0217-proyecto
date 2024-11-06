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



 
/* Funcionamiento para no perderme al hacer los menús xD
 Agregar los otros displays
 Para el caso de que se ingrese la opcion 1: Ingresar al sistema 
___________________________________________________________________________________________________
 Caso ingresar al sistema: 
    1. Atención cliente:
        - Despliega otro menú en el cual se pueden ver todas las opciones de atencion al cliente
    2. Información general de Préstamos:
        - Despliega el menu con las opciones establecidas para este.
___________________________________________________________________________________________________
 Caso de crear una cuenta: 
 -> Despliega menú con los requerimientos 
    1. Nombre: (Recibe)
    2. Tipo de cuente: (Recibe la opción)
        - 1. Colones 
        - 2. Dólares
        - 3. Ambas
    -> Imprime el id de cuenta 
    -> Imprimir el cvv 
    -> Imprimir el PIN
___________________________________________________________________________________________________
*/
