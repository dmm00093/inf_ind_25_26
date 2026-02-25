// 1: Programa basico: crear array, leer array, modificar array.

#include <stdio.h>

int main ()
{
    int numbers [] = {0, 1, 2, 3, 4};

    int i;

    printf("Listado de elementos: \n");

    for (i=0 ; i < 5 ; i++)
    {
        printf ("Numero %d : %d\n", i, numbers[i]);
    }

    printf("\n");

    printf("Cambio de elementos: \n");
    numbers[0] = 10; printf("Primer elemento: %d\n", numbers[0]);
    numbers[4] = 50; printf("Ultimo elemento: %d\n", numbers[4]);

    printf("\n");

    printf("Listado de nuevo array: \n");

    for (i=0 ; i<5 ; i++)
    {
        printf("Numero %d, %d\n", i, numbers[i]);
    }

    return 0;
}