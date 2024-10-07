// Autores: Carlota Moncasi - 839841
//          E. lilai Naranjo - 840091

#include "betris.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

/*
 * Pre: 1 <= n AND d <= h AND (h - d) <= RAND_MAX
 * Post: el vector vEntrada[0,n-1] almacena n datos del intervalo [a,b]
 *       generados de modo pseudoaleatorio
 */
int numAleatorio(const int a, const int b) {

    return a + rand() % (b - a + 1);

}

/*
 * Post: limpia todo lo que hay en pantalla
 */
void borrarPantalla () {

    cout << "\033[2J";

}

int main(int argc, char* argv[]) {

    tpTablero tablero;
    int vEntrada[MAXENTRADA];
    int vSalida[MAXENTRADA];
    int numPiezas = 0;

    tablero.nfils = atoi(argv[1]);
    tablero.ncols = atoi(argv[2]);
    int objetivo = atoi(argv[3]);
    int retardo = atoi(argv[4]);

    // guardamos en vEntrada las piezas mostradas en la invocación del programa
    for (unsigned i = 0; argv[i+5] > 0; i++) {

        vEntrada[i] = atoi(argv[i+5]);

    }

    int fin = vEntrada[0];  // primera pieza de vEntrada
    if (fin < 0) {      // la primera pieza es negativa

        time_t semilla = time(nullptr);     // tiempo actual como semilla
        srand(semilla);     // inicializa el generador de números pseudoaleatorios

        for (int i = 0; i < -fin; i++) {    // -fin es el número de piezas de vEntrada

            // asigna a vEntrada[i] un pseudoaleatorio del intervalo [0,4]
            vEntrada[i] = numAleatorio(0, 4);
            vEntrada[-fin] = -1;    // última pieza es -1 para terminar 

        }

    }

    borrarPantalla();
    inicializarTablero(tablero);

    numPiezas = buscaSolucion(tablero, vEntrada, vSalida, objetivo, 0, retardo);

    if (retardo < 0) {  // se muestra el tablero en su estado final

        mostrarTablero(tablero, vEntrada);

    }

    cout << "\nEntrada (números de piezas): ";
    int i = 0;

    while (vEntrada[i] != -1) {

        cout << vEntrada[i] << " ";
        i++;

    }

    if(numPiezas == -1) {       // el programa no tiene solución

        borrarPantalla();
        
    }

    cout << "\nNúmero de piezas colocadas: " << numPiezas << endl;
    cout << "Salida (número de columnas): ";

    if (numPiezas != -1) {

        i = 0;

        while (i < numPiezas) {

            cout << vSalida[i] << " ";
            i++;

        }

    }

    else {

        cout << " ";

    }
    
    cout << endl;

}
