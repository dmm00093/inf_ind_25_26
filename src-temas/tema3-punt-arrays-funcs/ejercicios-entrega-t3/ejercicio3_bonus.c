/* Bonus
Implementa void reverse_int(int *v, size_t n); (invierte in-place).
Implementa int busca_int(const int *v, size_t n, int objetivo); (devuelve índice o -1)
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void reverse_int(const int *v, int *u, size_t n) // Primera forma, sin malloc. Simplemente devuelve un cambio.

// Lo malo de esto es que necesitamos un segundo array u. Este se declara en main con longitud n
// Tendra la misma longitud que nuestro input v, y simplemente se ira escribiendo al reves.

{
    for (int i = n - 1; i >= 0; i--)
    {
        u [i] = v[(n-1) - i]; // Podemos usar un bucle for inverso. Y se lee al reves asi, simple
    }
    // No devuelve nada es un void
} // Este es sin usar malloc. Mas facil.

int* reverse_int_malloc(const int *v, size_t n) // usamos malloc. hay que hacer free luego para memoria.
{
    int *arrayInv = malloc (n * sizeof (int)); // asigna ese array a un tamaño n con malloc
    if (!arrayInv) return NULL;

    for (int i = n - 1; i >= 0; i--)
    {
        arrayInv [i] = v[(n-1) - i]; // igual
    }

    return arrayInv;
} // Usando malloc. int debe ser puntero.

int busca_int (const int *v, size_t n, int objetivo)
{
    int num = -1; // tenemos una variable que es si no coincide

    for (int i = 0; i < n; i++)
    {
        if (v[i] == objetivo) return i; // si coincide pues devolvemos el indice en v[i] y termina la func.
    }

    return num; // si no existe pues devolvemos -1

} // Buscar indice de un numero especificado

int main ()
{
    printf("Introduce longitud de tu array: "); int l; scanf("%d", &l); // leemos longitud

    printf("Introduce tu array:\n"); int array [l];

    for (int i = 0; i < l; i++)
    {
        printf("Numero %d: ", i); scanf("%d", &array[i]); // escribimos array
    }

    printf("Tu array es:\n[");
    for (int i = 0; i < l; i++)
    {
        printf("%d", array[i]);
        if (i < l - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");

    int arrayInv [l]; // aqui es donde necesitamos ese segundo array para la funcion

    reverse_int(array, arrayInv, l); // es un void, ataca directamente a arrayinv, o u en nuestra func

    printf("Tu array invertido (sin malloc) es:\n[");
    for (int i = 0; i < l; i++)
    {
        printf("%d", arrayInv[i]);
        if (i < l - 1)
        {
            printf(", "); // imprime
        }
    }
    printf("]\n");

    int *arrayInvB = reverse_int_malloc (array, l); // con malloc tiene que ser puntero

        if (!arrayInvB)
        {
            printf("ERR1 - No se pudo alocar memoria");
            return 1;
        } // si no se puede alocar memoria devolvemos error 1

    printf("Tu array invertido (con malloc) es:\n[");
    for (int i = 0; i < l; i++)
    {
        printf("%d", arrayInvB[i]);
        if (i < l - 1)
        {
            printf(", "); // imprime
        }
    }
    printf("]\n");

        free(arrayInvB);

    printf("\nIntroduce un numero del array: "); int indice; scanf("%d", &indice);

    printf("El indice del numero [%d] dentro de array es: [%d]", indice, busca_int(array, l, indice));

    return 0;
}

