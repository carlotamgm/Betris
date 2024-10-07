// Autores: Carlota Moncasi - 839841
//          E. Lilai Naranjo - 840091

#include "betris.hpp"
#include <iostream>
#include <ctime>
using namespace std;

/*
 * Pre: true
 * Post: Todas las componentes de tablero.matriz son -1 (casilla vacía)
 */
void inicializarTablero(tpTablero &tablero) {

    for (unsigned i = 0; i < MAXDIM; i++) {

        for (unsigned j = 0; j < MAXDIM; j++) {

            tablero.matriz[j][i] = -1;

        }

    }

}

// Pre: vEntrada contiene los números de las piezas que entran y tablero contiene el estado actual 
//      del tablero, en el que pueden estar colocadas algunas de dichas piezas 
// Post: Se ha mostrado el tablero por pantalla (cada pieza se ha dibujado con su color)
void mostrarTablero(const tpTablero & tablero, const int vEntrada[MAXENTRADA]) {

    cout << "\033[1;1H";        // sube el cursor a la esquina superior izquierda de la pantalla

    for (int fila = 0; fila < tablero.nfils; fila++) {

        for (int col = 0; col < tablero.ncols; col++) {

            if (tablero.matriz[fila][col] == -1) {      // no hay ninguna pieza en la posición actual

                cout <<  " " ;
                
            } 
            
            else {        // hay alguna pieza en la posición actual

                cout << "\033[" << vPiezas[vEntrada[tablero.matriz[fila][col]]].color 
                     << "m" << " " << "\033[0m";

            }

        }

        cout << endl;

    }

}

/*
 * Pre: true
 * Post: devuelve false si hay huecos en "fila" y true en caso contrario
 */
bool estaLlena(tpTablero tablero, int fila) {

    for (int col = 0; col < tablero.ncols; col++) {

        if (tablero.matriz[fila][col] == -1) {      // hay un hueco (está vacía)

            return false;

        }

    }

    return true;

}

/*
 * Pre: true
 * Post: devuelve true si se han colocado piezas de tal forma que se han completado "objetivo" filas 
         sin huecos y false en caso contrario
 */
bool objetivoCumplido(tpTablero tablero, const int objetivo) {

    int filaCompleta = 0;

    for (int fila = 0; fila < tablero.nfils; fila++) {

        filaCompleta += estaLlena(tablero, fila);       // estaLlena devuelve 1 o 0

    }

    return filaCompleta == objetivo;
 

}

/*
 * Pre: true
 * Post: devuelve true si todas las posiciones que componen la forma de la pieza que queremos colocar están vacías
         y false en caso contrario
 */
bool entraPieza(tpTablero &tablero, const int vEntrada[MAXENTRADA], int posicion, int fila, int vSalida[MAXENTRADA]) {

    for (int i = 0; i < TAMPIEZA; i++) {

        // las posiciones donde queremos colocar la pieza no están vacías 
        // o alguna parte de la pieza se encuentra fuera de los límites del tablero
        if (tablero.matriz[fila + vPiezas[vEntrada[posicion]].forma[i][0]][vSalida[posicion] + vPiezas[vEntrada[posicion]].forma[i][1]] != -1
            || fila + vPiezas[vEntrada[posicion]].forma[i][0] >= tablero.nfils 
            || vSalida[posicion] + vPiezas[vEntrada[posicion]].forma[i][1] >= tablero.ncols) {

            return false;

        }

    }

    return true;

}

/*
 * Pre: "posicion" es la posición que ocupa una pieza en el vector vEntrada
 * Post: escribe "posicion" en las posiciones correspondientes del tablero haciendo la forma de la pieza
 */
void ponerPieza(tpTablero &tablero, const int vEntrada[MAXENTRADA], int vSalida[MAXENTRADA], int posicion, int fila) {

    for (int i = 0; i < TAMPIEZA; i++) {

        tablero.matriz[fila+vPiezas[vEntrada[posicion]].forma[i][0]][vSalida[posicion]+vPiezas[vEntrada[posicion]].forma[i][1]] = posicion;

    }

}

/*
 * Pre: "posicion" es la posición que ocupa una pieza en el vector vEntrada
 * Post: escribe -1 en las posiciones correspondientes del tablero haciendo la forma de la pieza
 */
void quitarPieza(tpTablero &tablero, const int vEntrada[MAXENTRADA], int posicion, int fila, int vSalida[MAXENTRADA]) {

    if (tablero.matriz[fila][vSalida[posicion]] != posicion) {       // no estás en la fila de la pieza que quieres quitar

        quitarPieza(tablero, vEntrada, posicion, fila+1, vSalida);

    } else {      // estás en la fila de la pieza que quieres quitar

        for (int i = 0; i < TAMPIEZA; i++) {

            tablero.matriz[fila+vPiezas[vEntrada[posicion]].forma[i][0]][vSalida[posicion]+vPiezas[vEntrada[posicion]].forma[i][1]] = -1;

        }

    }

}

/*
 * Pre: en tablero se han colocado las n primeras piezas de vEntrada, en las columnas indicadas respectivamente en vSalida
 * Post: devuelve la fila del tablero en la que se puede colocar una pieza 
 *       (mirando desde arriba, es la primera fila en la que cabe la pieza)
 */
int dejarCaerPieza(tpTablero &tablero, const int vEntrada[MAXENTRADA], int n, int vSalida[MAXENTRADA]) {

    int fila = 0;

    // se puede colocar la pieza en la posición en la que estamos y "fila" no se sale del tablero
    while (entraPieza(tablero, vEntrada, n, fila, vSalida) && fila < tablero.nfils) {

        fila++;

    }
    
    return fila-1;

}

/*
 * Pre: se ha mostrado el tablero actual
 * Post: espera "retardo" milisegundos antes de realizar la siguiente acción
 */
void wait (int retardo) {

    clock_t t1 = clock(), t2 = clock();

    while (1000*int(t2-t1)/CLOCKS_PER_SEC < retardo) {

        t2 = clock();

    }

}

/*
 * Pre: se ha puesto o se ha quitado una pieza 
 * Post: si el retardo es positivo, muestra el tablero actualizado 
 *       y espera "retardo" milisegundos antes de realizar la siguiente acción
 */
void cambiarEstado(tpTablero &tablero, const int vEntrada[MAXENTRADA], const int retardo) {

    if (retardo >= 0) {

        mostrarTablero(tablero, vEntrada);
        wait(retardo);

    }

}


/*
 * Pre: se han colocado n piezas en el tablero o hemos llegado a la última pieza, y el objetivo no se ha cumplido       
 * Post: quita la pieza anterior y cambia su columna para después buscar nueva solución
 */
void marchaAtras(tpTablero &tablero, const int vEntrada[MAXENTRADA], int vSalida[MAXENTRADA], int n, int fila, const int retardo) {

    quitarPieza(tablero, vEntrada, n-1, fila, vSalida);    // quitamos la pieza anterior
    cambiarEstado(tablero, vEntrada, retardo);
    vSalida[n-1]++;     // miramos en la columna siguiente
    
}



/*
 * Pre: en el tablero se han colocado las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
 * Post: si las piezas colocadas completan al menos objetivo filas sin huecos, entonces devuelve el número de piezas colocadas, 
 *       en vSalida las columnas en las que se han colocado las piezas y el tablero con las piezas colocadas. Si no, devuelve -1
 */
int buscaSolucion(tpTablero &tablero, const int vEntrada[MAXENTRADA], int vSalida[MAXENTRADA], const int objetivo, int n, const int retardo) {   

    // el objetivo está cumplido o el índice de pieza se ha salido de vEntrada
    // porque el programa ha terminado y se han puesto todas las piezas en el tablero
    if (objetivoCumplido(tablero, objetivo) || n == -1) {

        mostrarTablero(tablero, vEntrada);
        return n;

    } else if (vEntrada[n] == -1) {       // el objetivo no está cumplido y hemos llegado a la última pieza

        marchaAtras(tablero, vEntrada, vSalida, n, 0, retardo); 
        // llamamos recursivamente a la función para la pieza anterior
        return buscaSolucion(tablero, vEntrada, vSalida, objetivo, n-1, retardo);

    } else {      // objetivo no cumplido y aún quedan piezas por poner

        while (vSalida[n] < tablero.ncols) {

            int fila = dejarCaerPieza(tablero, vEntrada, n, vSalida);

            if (fila == -1) {       // no caben más piezas en la columna en la que estamos
 
                if (vSalida[n] == tablero.ncols-1) {        // estamos en la última columna del tablero

                    vSalida[n] = 0;     // la pieza vuelve a la primera columna
                    marchaAtras(tablero, vEntrada, vSalida, n, fila, retardo);
                    // la llamada recursiva busca una nueva solución para la pieza anterior
                    return buscaSolucion(tablero, vEntrada, vSalida, objetivo, n-1, retardo);

                } else {      // aún no hemos llegado a la última columna del tablero

                    vSalida[n]++;       // busca nueva solución en la columna siguiente

                }

            } else {      // caben piezas en la columna en la que estamos

                ponerPieza(tablero, vEntrada, vSalida, n, fila);
                cambiarEstado(tablero, vEntrada, retardo);
                // llamamos recursivamente a la función para la pieza siguiente
                return buscaSolucion(tablero, vEntrada, vSalida, objetivo, n+1, retardo);

            }

        }

        // hemos probado todas las posibilidades y no se cumple el objetivo
        if (!objetivoCumplido(tablero, objetivo)) {

            return -1;

        }

    }

}
