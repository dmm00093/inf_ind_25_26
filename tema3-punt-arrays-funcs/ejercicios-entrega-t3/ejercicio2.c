// Created by dmm00093 on 23/02/2026.

// 2: Swap y test

// A) Implementa swap(int*, int*) y B) Probar con varios numeros (scanf)

#include <stdio.h>

void swap(int *a, int *b) // Funcion para intercambiar numeros. Con ints.
{
    int var1 = *a; int var2 = *b; // Cambio de numeros

    *a = var2; *b = var1;
}

void swapDouble(double *a, double *b) // Funcion para intercambiar numeros. Con ints.
{
    double var1 = *a; double var2 = *b; // Cambio de numeros

    *a = var2; *b = var1;
}

int main(void)
{
    int x; int y; // no establecerlos como un double va a producir un eror en el programa.

    printf("Introduzca su primer numero {x}: ");  scanf("%d", &x);
    printf("Introduzca su segundo numero {y}: "); scanf("%d", &y);

    swap(&x, &y);
    printf("Swap realizado --> {x=%d}, {y=%d} ||\n", x, y); // esperado: x=2 y=1

    /* No tengo muy claro por que esto no funciona. Porque cuando hago el scanf me salta algo diferente.

    double w; double z; // con doubles

    printf("\nAhora con decimales:\n ");

    printf("Introduzca su primer numero {w}: ");  scanf("%.3f", &w);
    printf("Introduzca su segundo numero {z}: "); scanf("%.3f", &z);

    swapDouble(&w, &z);
    printf("Swap realizado --> {w=%.3f}, {z=%.3f} ||\n", w, z);
    */
    return 0;
}