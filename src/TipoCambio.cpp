#include "TipoCambio.hpp"

double generarTipoCambio(double min, double max){
    // Generar un número simple en el rango de mínimo y máximo
    double randomFact = (1.0 * (max - min)) / 10; // Simular una varianza 
    return min + randomFact;  
}