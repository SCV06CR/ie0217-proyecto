#include <regex>
#include "InformacionPrestamos.hpp"
#include "baseDatos.hpp"

using namespace std;

// Genera el id random con un prefijo (410 colones, 420 dolares)
int generarPrestamoID(int prefijo) {
    /* Inicializacion de la semilla aleatoria: */
    srand(time(NULL));

    return prefijo * 100000 + rand() % 100000;
}

// Sub menu de gestion de prestamos
// Recibe el id de la cuenta
void showMenuSP(const int& idCuenta, sqlite3* db) {
    int opcion;

    do {
        try {
            cout << "Ingrese el servicio que desea recibir: " << endl;
            cout << "1. Solicitar un préstamo." << endl;
            cout << "2. Generar reporte de pagos estimados." << endl;
            cout << "3. Regresar al menú anterior." << endl;
            cout << "Ingrese su selección: ";
            cin >> opcion;

            // Validación de entrada
            if (cin.fail() || opcion < 1 || opcion > 3) {
                throw invalid_argument("Opción inválida. Debe ingresar un número entre 1 y 3.");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer

            switch (opcion) {
                case 1:
                    gestionarPrestamo(db, idCuenta);
                    break;
                case 2:
                    reportePrestamos(db);
                    break;
                case 3:
                    cout << "Regresando al menú anterior..." << endl;
                    break;
                default:
                    // Este caso no debería ocurrir debido a la validación previa
                    cout << "Opción no válida. Por favor, intente de nuevo." << endl;
            }
        } catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar el buffer
        }
    } while (opcion != 3);
}


// Sub menu de solicitud de prestamos
void gestionarPrestamo(sqlite3* db, const int& idCuenta) {
    int opcionPrestamo;
    double montoColones;
    double montoDolares;

    // si los primeros numeros son 100 es cuenta en colones 
    // Si los primeros numeros son 200 es cuenta en dolares
    // para separar los primeros tres dijitos del id 
    int colones = 100; 
    int dolares = 200; 

    std::string idStr = std::to_string(idCuenta); //convertir a string
    std::string digits = idStr.substr(0, 3); // extraer los primeros 3 caracteres


        // Menú de opciones
        cout << "Seleccione el tipo de prestamo a solicitar:\n"
             << "1. Prestamo Personal\n"
             << "2. Prestamo Prendario\n"
             << "3. Prestamo Hipotecario\n"
             << "4. Regresar al menú anterior.\n";

        int tipoPrestamo;

        cin.ignore(); // Limpiar el buffer de entrada
        cin >> tipoPrestamo;

         if (stoi(digits) == colones) {
            cout << "\nIngrese el monto en colones del Prestamo: ";
            cin >> montoColones;

        } else if(stoi(digits) == dolares) {

            cout << "\nIngrese el monto en dolares del Prestamo: ";
            cin >> montoDolares;
        }

        double interes;
        int plazoPrestamo;

        // Tipos de prestamos disponibles
        string prestamoPersonal = "Prestamo Personal";
        string prestamoPrendario = "Prestamo Prendario";
        string prestamoHipotecario = "Prestamo Hipotecario";

        switch (tipoPrestamo) {
            case 1:
                if (stoi(digits) == colones){
                    cout << "Seleccione una opcion para Prestamo Personal en Colones\n"
                         << "1. Tasa de interes anual del prestamo: 15% y Plazo en meses: 24\n"
                         << "2. Tasa de interes anual del prestamo: 25% y Plazo en meses: 48\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 15;
                        plazoPrestamo = 24;
                    } else if (opcionPrestamo == 2) { 
                        interes = 25;
                        plazoPrestamo = 48;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores correspondientes
                    solicitarPrestamo( db, "Prestamos_Colones", idCuenta, interes, montoColones, plazoPrestamo, prestamoPersonal);
                    
                } else if (stoi(digits) == dolares){
                    cout << "Seleccione una opcion para Prestamo Personal en Dolares\n"
                         << "1. Tasa de interes anual del prestamo: 17% y Plazo en meses: 24\n"
                         << "2. Tasa de interes anual del prestamo: 27% y Plazo en meses: 48\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 17;
                        plazoPrestamo = 24;
                    } else if (opcionPrestamo == 2) { 
                        interes = 27;
                        plazoPrestamo = 48;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Dolares", idCuenta, interes, montoDolares, plazoPrestamo, prestamoPersonal);
                }
                
                break;

            case 2:
                if (stoi(digits) == colones){
                    cout << "Seleccione una opcion para Prestamo Prendario en Colones\n"
                         << "1. Tasa de interes anual del prestamo: 10% y Plazo en meses: 36\n"
                         << "2. Tasa de interes anual del prestamo: 18% y Plazo en meses: 60\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 15;
                        plazoPrestamo = 24;
                    } else if (opcionPrestamo == 2) { 
                        interes = 25;
                        plazoPrestamo = 48;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Colones", idCuenta, interes, montoColones, plazoPrestamo, prestamoPrendario);
                    
                } else if (stoi(digits) == dolares){
                    cout << "Seleccione una opcion para Prestamo Prendario en Dolares\n"
                         << "1. Tasa de interes anual del prestamo: 12% y Plazo en meses: 36\n"
                         << "2. Tasa de interes anual del prestamo: 20% y Plazo en meses: 60\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 12;
                        plazoPrestamo = 36;
                    } else if (opcionPrestamo == 2) { 
                        interes = 20;
                        plazoPrestamo = 60;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Dolares", idCuenta, interes, montoDolares, plazoPrestamo, prestamoPrendario);
                }
                break;

            case 3:
                if (stoi(digits) == colones){
                    cout << "Seleccione una opcion para Prestamo Hipotecario en Colones\n"
                         << "1. Tasa de interes anual del prestamo: 17% y Plazo en meses: 60\n"
                         << "2. Tasa de interes anual del prestamo: 25% y Plazo en meses: 84\n" ;
                    cin >> opcionPrestamo;

                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 15;
                        plazoPrestamo = 24;
                    } else if (opcionPrestamo == 2) { 
                        interes = 25;
                        plazoPrestamo = 48;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Colones", idCuenta, interes, montoColones, plazoPrestamo, prestamoHipotecario);
                    
                } else if (stoi(digits) == dolares){
                    cout << "Seleccione una opcion para Prestamo Hipotecario en Dolares\n"
                         << "1. Tasa de interes anual del prestamo: 20% y Plazo en meses: 60\n"
                         << "2. Tasa de interes anual del prestamo: 30% y Plazo en meses: 84\n" ;
                    cin >> opcionPrestamo;
                    
                    // Se asignan los valores de tasa de interes y plazo en meses
                    if (opcionPrestamo == 1) {
                        interes = 20;
                        plazoPrestamo = 60;
                    } else if (opcionPrestamo == 2) { 
                        interes = 30;
                        plazoPrestamo = 84;
                    } else {
                        cout << "Opcion ingresada no valida\n";
                        break;
                    }
                    // Registra en la base de datos los valores ingresados
                    solicitarPrestamo( db, "Prestamos_Dolares", idCuenta, interes, montoDolares, plazoPrestamo, prestamoHipotecario);
                }
                break;

            case 4:
                cout << "Regresando..." << endl;
                break;

            default:
                cerr << "Opcion invalida.\n";
                return;
        }

}

// Inserta en la tabla los datos para el nuevo prestamo
// Recibe el ID de la cuenta y la base de datos
// Recibe los datos seleccionados por el usuario para insertar
void solicitarPrestamo(sqlite3* db, const string& tabla,const int& idCuenta, double& interes, double& monto, int& plazoPrestamo, const string& tipoPrestamo) {
    // si los primeros numeros son 100 es cuenta en colones 
    // Si los primeros numeros son 200 es cuenta en dolares
    // para separar los primeros tres dijitos del id 
    int colones = 100; 
    int dolares = 200; 

    std::string idStr = std::to_string(idCuenta); //convertir a string
    std::string digits = idStr.substr(0, 3); // extraer los primeros 3 caracteres
    int idPrestamo;
    string query;

    // Se determina la moneda para el prestamo
    if (stoi(digits) == colones){
        idPrestamo = generarPrestamoID(410);
        // Se mueve el monto en colones a la cuenta correspondiente
        query = "UPDATE Cuenta_Colones SET cantidad_dinero = cantidad_dinero + " + to_string(monto) + " WHERE id = " + to_string(idCuenta) + ";";
    } else if (stoi(digits) == dolares){
        idPrestamo = generarPrestamoID(420);
        // Se mueve el monto  en dolares a la cuenta correspondiente
        query = "UPDATE Cuenta_Dolares SET cantidad_dinero = cantidad_dinero + " + to_string(monto) + " WHERE id = " + to_string(idCuenta) + ";";
    }

    // Se calcula la cuota mensual
    // Cuota Mensual = (Capital * Taza Interes Mensual) / (1 - (1 + Taza Interes Mensual)^(-Plazo en Meses));
    double interesMensual = (interes) / 12 / 100;
    double cuotaMensual = (monto * interesMensual) / (1 - pow(1 + interesMensual, -plazoPrestamo));
    // Se redondea la cifra y se limita a dos decimales
    cuotaMensual = round(cuotaMensual * 100) / 100; 

    // Se insertan los datos en la tabla correspondiente
    string sql = "INSERT INTO " + tabla + " (id_prestamo, id_cuenta, intereses, meses, monto, intereses_abonados, saldo_restante, tipo_prestamo, monto_por_cuota, cuotas_pagadas) VALUES (" + to_string(idPrestamo) + ", " + to_string(idCuenta) + ", " + to_string(interes) + ", " + to_string(plazoPrestamo) + ", " + to_string(monto) + ", 0, " + to_string(monto) + ",'" + tipoPrestamo + "', " + to_string(cuotaMensual) + ", 0);";

    ejecutarSQL(db, sql.c_str());


    // Preparar consulta de sql 
    sqlite3_stmt* stmt; 
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl; 
        return;
    }

    // Ejecutar la consulta
    if(sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "Error al ejecutar la consulta: " << sqlite3_errmsg(db) << endl; 
    } else {
        cout << fixed << "Deposito de " << monto << " a la cuenta: " << idCuenta << ", realizado con éxito. \n"; 
    }

    // Limpiar el statement
    sqlite3_finalize(stmt); 

    // Se llama a la funcion para imprimir los datos del prestamo
    cout << "\n";
    consultaPrestamo(db, idPrestamo, tabla);
    cout << "\n";
}

// Se imprime al usuario los datos del prestamo que solicito
// Recibe la base de datos, el id del prestamo y la tabla a la cual pertenece
void consultaPrestamo(sqlite3* db, int &idPrestamo, const string& tabla) {
    string idStrPrestamo = std::to_string(idPrestamo);
    string sql;

    // Se determina de que tabla se va a leer la informacion
    if (tabla == "Prestamos_Colones") {
        sql = "SELECT * FROM Prestamos_Colones WHERE id_prestamo = ?";
    } else if(tabla == "Prestamos_Dolares") {
        sql = "SELECT * FROM Prestamos_Dolares WHERE id_prestamo = ?";
    }

    // Preparar consulta de sql
    sqlite3_stmt* stmt; 
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        
        cout << "Prestamo tramitado exitosamente\n";

        sqlite3_bind_text(stmt, 1, idStrPrestamo.c_str(), -1, SQLITE_STATIC); // Vincular el id a ? en la consulta

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
        return;
    }

    // Limpiar el statement
    sqlite3_finalize(stmt); 
}

// Funcion para generar un reporte de un prestamo determinado
void reportePrestamos(sqlite3* db) {

    // Datos de la tabla de prestamo
    string strIDPrestamo;
    double interesAnual;
    double capital;
    int cuotasPagadas;
    double saldoRestante;
    double interesesAbonados;
    double cuotaMensual;

    // Se verifica que un ID de prestamo ingresado exista
    do {
        cout << "Ingrese el del ID prestamo que desea consultar: \n";
        cin.ignore();
        getline(cin, strIDPrestamo);
    } while (!verificarPrestamo(db, strIDPrestamo));

    // si los primeros numeros son 410 es un prestamo en colones 
    // Si los primeros numeros son 420 es un prestamo en dolares
    // para separar los primeros tres dijitos del id 
    int colones = 410; 
    int dolares = 420; 

    string digits = strIDPrestamo.substr(0, 3); // extraer los primeros 3 caracteres

    // Se determina la moneda del prestamo
    string tabla;
    if (stoi(digits) == colones){
        tabla = "Prestamos_Colones";
    } else if (stoi(digits) == dolares){
        tabla = "Prestamos_Dolares";
    }

    // Se extraen los datos necesarios de la tabla de prestamos
    interesAnual = extraerDatoDouble(db, strIDPrestamo, tabla, "intereses");
    capital = extraerDatoDouble(db, strIDPrestamo, tabla, "monto");
    cuotasPagadas = extraerDatoEntero(db, strIDPrestamo, tabla, "cuotas_pagadas");
    saldoRestante = extraerDatoDouble(db, strIDPrestamo, tabla, "saldo_restante");
    interesesAbonados = extraerDatoDouble(db, strIDPrestamo, tabla, "intereses_abonados");
    cuotaMensual = extraerDatoDouble(db, strIDPrestamo, tabla, "monto_por_cuota");
    
    // Cuota Interes Mensual = (Cuotas Restantes * Interes Mensual) / 12
    double cuotaInteresMensual = (saldoRestante * (interesAnual / 100)) / 12;
    
    // Calculo del aporte al capital
    double aporteCapital = capital - saldoRestante;

    cout.precision(2);
    cout << fixed
         << "\nReporte de prestamos\n"
         << "ID del prestamo: " << strIDPrestamo << "\n"
         << "Capital inicial: " << capital << "\n"
         << "Saldo restante: " << saldoRestante << "\n"
         << "Cuota mensual con interes: " << cuotaMensual << "\n"
         << "Cuota de interes mensual: " << cuotaInteresMensual << "\n"
         << "Cuotas Pagas: " << cuotasPagadas << "\n"
         << "Aporte al capital: " << aporteCapital << "\n"
         << "Intereses Abonados: " << interesesAbonados << "\n"
         << "\n\n";
}

// Funcion para extraer un dato entero de la tabla segun el id del prestamo
int extraerDatoEntero(sqlite3* db,string &strIDPrestamo, string tabla, string columna) {

    int dato;

    // Se selecciona el dato correspondiente de la columna
    string sql = "SELECT " + columna + " FROM " + tabla +" WHERE id_prestamo = " + strIDPrestamo + ";";
    
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &checkStmt, 0);
    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
        // Se almacena el dato
        dato = sqlite3_column_int(checkStmt, 0); 
    } else {
        return 1;
    }
    sqlite3_finalize(checkStmt);

    return dato;
}

// Funcion para extraer un dato double de la tabla segun el id del prestamo
double extraerDatoDouble(sqlite3* db,string &strIDPrestamo, string tabla, string columna) {

    double dato;

    // Se selecciona el dato correspondiente de la columna
    string sql = "SELECT " + columna + " FROM " + tabla +" WHERE id_prestamo = " + strIDPrestamo + ";";
    
    sqlite3_stmt* checkStmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &checkStmt, 0);
    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
        // Se almacena el dato
        dato = sqlite3_column_double(checkStmt, 0);
    } else {
        return 1;
    }
    sqlite3_finalize(checkStmt);

    return dato;
}