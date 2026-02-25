// 4 - Programa que realize la suma de todos los elementos de un array

#include <stdio.h>

int main () {

    // Bloque Lectura:
    // Deberia hacerse una funcion.

    printf ("Escriba la longitud del array:");

    int l; scanf("%d", &l);

    if (l <= 0) // Aseguremos un array válido.
    {
        printf("ERR1: longitud NO puede ser %d\n", l); // aseguramos que sea valido
        return 1;
    }

    int array [l]; // Escaneamos longitud aray. Asignamos longitud a ese array.

    printf("Longitud de array: [%d]\n", l);

    printf("Escriba su array: \n");

    for (int i = 0 ; i < l ; i++) // Preguntamos al usuario que introduzca los numeros del array.
    {
        printf("Numero %d: ", i);
        scanf("%d", &array [i]);
    }

    // Bloque Suma (Nuevo)

    int x = 0;

    for (int i = 0 ; i < l ; i++)
    {
        x = x + array [i]; // pues almacenamos una variable nueva que inicia en 0 y se suma recursivamente
    }

    printf("Tu array es:\n["); // boom shaka laka. podemos jugar con los \n y los textos para dejarlo bonito

    for (int i = 0 ; i < l ; i++)
    {
        printf(" %d ", array[i]); // lista espaciada dentro de [
    }

    printf ("]\nLa suma de sus miembros es: %d", x); // termina la lista y ponemos el ] final y \n y texto nuevo de suma.
    // se puede jugar mucho con los printf y los \n. boom shaka laka.
    return 0;
}