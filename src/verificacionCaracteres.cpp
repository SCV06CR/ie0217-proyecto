#include "verificacionCaracteres.hpp"

// Funcion para validar el nombre
string verificarNombre()  {
    
    string nombre;
    bool nombreValido = false;
    do {
        cout << "\nIngresese su nombre completo: ";
        cin.ignore();
        getline(cin, nombre);

        nombreValido = !nombre.empty();
        for (char c : nombre) {
            if (!isalpha(c) && c != ' ') {
                nombreValido = false;
                break;
            }
        }

        if (!nombreValido) {
            cout << "Error: El nombre solo debe contener letras y espacios." << endl;
        }
    } while (!nombreValido);

    return nombre;
}

// Funcion para validar la contrasena
string verificarContrasena() {}