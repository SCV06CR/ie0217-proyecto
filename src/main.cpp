#include <iostream> 
#include "TipoCambio.hpp"

using namespace std; 

int main(){
    int option; 
    double TipoCambioMin = 520.0;
    double TipoCambioMax = 537.5;
    double TipoCambio = generarTipoCambio(TipoCambioMin, TipoCambioMax); 




    do{
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
        cout << "|                             3.Salir del sistema                           |" <<  endl;
        cout << "|---------------------------------------------------------------------------|" <<  endl;
        cin >> option;

        switch(option){
            // Opcion de ingresar al sistema 
            case 1: { 
                
                int idCuenta; 
                cout << "Ingrese el id de su cuenta: " << endl; 
                // Agregar verificacion de la cuenta existente en las db 
                cin >> idCuenta; 
                // Si la cuenta no existe mostrar opcion de que cuenta incorrecta.
                // Tal vez pedir una contraseña.
                int opcion1;
                do{
                    
                    cout << "Ingrese el servicio que desea recibir: " << endl; 
                    cout << "1. Atención al cliente.\n2. Información general sobre Préstamos.\n3. Devolverse" << endl;
                    cout << "Ingrese su selección: "; 
                    cin >> opcion1; 

                    switch (opcion1){
                        case 1: 
                            //Agregar el display del menu de Atención al cliente
                            cout << "\nAquí se desplegará el menú de Atención al cliente\n" << endl; 
                        case 2: 
                            // Agregar el display de la info de prestamos
                            cout << "\nAquí se desplegará el menú de info de préstamos\n" << endl; 
                        case 3: 
                            cout << "\nAquí se devuelve al menú anterior" << endl; 
                            break;
                        // caso de opción no válida
                        default:
                            cout << "\nOpción inválida...\nPor favor intente de nuevo.\n" << endl; 

                    } 

                } while (opcion1 != 3);
            } 
            case 2: {
                char nombreUser;
                char contrasena;
                char verifContrasena;
                int option2;
                cout << "\nPara crear una cuenta se requieren los siguientes datos: \n1. Nombre completo.\nSeleccionar tipo de cuenta.\nCrear una contraseña. " << endl; 
                cout << "\n1. Ingrese su nombre completo: " << endl; 
                cin >> nombreUser;
                //Esta bien usar tanto do??
                do{
                    cout << "\n2. Favor ingrese la contraseña que desea asignar" << endl; 
                    cin >> contrasena;
                    cout << "\nFavor vuelva a ingresar la contraseña para verificar que sean iguales" << endl;
                    cin >> verifContrasena; 
                }while(contrasena != verifContrasena);

                cout << "\nIngrese el tipo de cuenta que desea tener:\1. Cuenta en colones.\n2. Cuenta en dólares.\n3. Cuenta en ambas monedas." << endl; 
                cin >> option2; 
                // LLamar a la funcion para cuardar los elementos en la DB @SCV06CR

                cout << "\nImprimir los datos de la cuenta" << endl;
                // Imprimir numero de cuenta
                // Imprimir CVV
                // Imprimir Pin
                // Mostrar por un lado en colones y al otro en dolares (Si fuera creacion de ambas)
            }
            case 3:{
                cout << "\nSaliendo del programa...\nGracias por su visita." << endl;
                break; 
            } 
            default: 
                cout << "\nOpción inválida... \n\nFavor ingrese una opción correcta.\n\n";
        } 

} while (option != 3);
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
