// 2: Programa que lee un array con scanf y lo imprime.

#include <stdio.h>

int main ()
{
    int array [5]; // definimos array

    int i;

    printf("Escriba los numeros del array: \n");

    for (i=0 ; i<5 ; i++) // un bucle for con scanf, cuidado, delimitar bien.
    {
        printf("Numero %d: ", i);
        scanf("%d", &array[i]); // Lee numero y lo mete al array. Esto no es seguro, puede rebosar
        // hay que usar otra cosa pero por ahora, esto sirve.

        // Si te das cuenta, scanf utiliza la direccion de memoria para leer. Mucho cuidado, punteros.
    }

    printf("\n");

    printf("Aquí está tu array:\n");

    for (i=0 ; i<5; i++)
    {
        printf("Numero %d : %d\n", i, array[i]); //
    }

    return 0;
}