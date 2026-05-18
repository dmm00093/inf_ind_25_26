// Created by dmm00093 on 23/02/2026.

/* 3) Vectores
Construir un pequeño “módulo” de utilidades de arrays:
int max_int(const int *v, size_t n);
int min_int(const int *v, size_t n);
double media_int(const int *v, size_t n);
void normaliza(double *v, size_t n); (deja el vector con media 0 y varianza 1, si es posible)
Aprendizajes: punteros, paso de parámetros, arrays en funciones, const
*/

#ifndef STATS_H
#define STATS_H

#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "stats.h"

// A : Funcion para ver el num mas grande de un array

int max_int (const int *v, size_t n) // const int *v es el vector desreferenciado, size_t n es la longitud especificada
{
    int i; int numMax = v[0]; // debemos empezar en 0 para ir comparando

    for (i = 0 ; i < n ; i++)
    {
        // numMax = v[i]; Esto pensaba que funcionaria. Pero cada iteración estoy cambiando su valor y no sirve...

        if (v[i] > numMax)
        {
            numMax = v[i]; // si el elemento del vector es mas grande, pues numMax sera ese valor
                           // aqui si funciona. ya que no reseteamos numMax cada momento :)
        }
    }

    return numMax;
}

// B : Funcion para ver el num mas chico de un array

int min_int (const int *v, size_t n)
{
    int i; int numMin = v[0];

    for (i = 0 ; i < n ; i++)
    {
        if (v[i] < numMin)
        {
            numMin = v[i]; // si el elemento del vector es mas chico, pues numMin sera ese valor
        }
    }

    return numMin;
}

// C : Funcion para obtener la media de un array

double media_int(const int *v, size_t n) // double ya que en int perdemos informacion
{
    int i; double numSuma = 0; // double si el array tiene decimales...

    for (i = 0 ; i < n ; i++)
    {
       numSuma += v[i]; // += es igual que numSuma = numSuma + v[i]
    }

    double numMedia = numSuma / n; // otro double

    return numMedia;
}

// D : Funcion para normalizar en el propio vector (media y desviacion)

/* void normaliza(double *v, size_t n)
{

}
*/

int main (void)
{
    // Bloque de prueba A)
    int l = 3;
    int array [] = {40, 5, 10}; // definimos lo necesario para el bucle y array

    printf("\nNuestro array es: [");

    int i;
    for (i = 0; i < l; i++) // Escribimos array
    {
        printf("%d", array[i]);

        if (i < (l-1))
        {
            printf(", "); // Busque como imprimir un array bien porque solo sabia hacer [ , , , ] y no quedaba bien...
            // Me parece una buena solucion
        }
    }

    printf("]\n");

    printf("\n****************\n");

    printf("A) El numero mas grande es: %d", max_int (array, l));

    // en C, un array usado en una expresión se convierte automáticamente en un puntero a su primer elemento? supongo

    printf("\n****************\n");

    printf("B) El numero mas chico es: %d", min_int (array, l));

    printf("\n****************\n");

    printf("C) La media es: %.2f", media_int (array, l));

    printf("\n****************\n");

    return 0;
}