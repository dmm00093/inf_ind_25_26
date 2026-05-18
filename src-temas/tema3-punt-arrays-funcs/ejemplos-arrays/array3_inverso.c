// 3 - Escribe un programa en C para leer n valores en un array y mostrarlos en orden inverso.

#include <stdio.h>

int main ()
{
    printf ("Escriba la longitud del array: ");

    int l; scanf("%d", &l); int array [l]; // Escaneamos longitud aray. Asignamos longitud a ese array.

    printf("Longitud de array: %d\n", l); printf("\n"); // Se puede poner \n al principio de un printf.
    // Asi evitamos tanto printf.

    printf("Escriba su array: \n");

    for (int i = 0 ; i < l ; i++) // Preguntamos al usuario que introduzca los numeros del array.
    {
        printf("Numero %d: ", i);
        scanf("%d", &array [i]);
    }

    printf("Array normal: \n");

    for (int i = 0 ; i < l ; i++)
    {
        printf("|| Numero %d: %d || ", i, array [i]); // Escribimos array normal
    }

    printf("\n"); printf("Array inverso:\n");

    int x;
    for (int i = 0 ; i < l ; i++)
    {
        x = (l-i)-1; // La longitud-1 sera nuestra nueva i, ya que cuenta desde 0, si l es 5, hay que iterar de (5-1) a 0
        printf("|| Numero %d: %d || ", x, array[x]); // igual
    }

    /* Podemos también utilizar un bucle diferente para nuestro bucle inverso:

    printf("Array inverso:\n");

    for (int i = l - 1; i >= 0; i--)
    {
        printf("|| Numero %d: %d || ", i, array[i]);
    }

    */

    return 0;
}