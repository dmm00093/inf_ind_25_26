/* Crear un programa que:
1. Pida al usuario el número de alumnos.
2. Reserve memoria dinámica para ellos.
3. Lea sus notas.
4. Calcule la media.
5. Libere la memoria.
Extensión:
Permitir redimensionar el vector con realloc */

// TODO: Hacer de nuevo para libreria lectura.h ya que el de scanf funciona sin problemas

#include <stdio.h>
#include <stdlib.h>
#include "lectura.h"

int main ()
{
    int alumnos;

    int testAlumn = leer_entero("Introduzca el numero de alumnos: ", &alumnos); printf("\n");

        if (testAlumn != 0) {
            printf("Error de lectura. Saliendo.");
            return -1;
        }

    float *notas = malloc(alumnos * sizeof(float));

        if (notas == NULL) {
            printf("Error de reserva de memoria.");
            getchar();
            return -1;
        }

    float suma = 0;

    for (int i = 0; i < alumnos; i++) {
        printf("Nota del alumno %d: ", i+1); int testNota = leer_float("", &notas[i]);

        if (testNota != 0) {
            printf("Error de lectura en alumno %d. Saliendo.", i+1);
            free(notas);
            getchar();
            return -1;
        }

        suma = suma + notas[i];
    }

    float media = suma / alumnos; printf("\nMedia total: %.2f", media);

    int alumnos_orig = alumnos;
    int alumnos_nuevos;

    printf("\nIntroduce tus nuevos alumnos:");
    scanf("%d", &alumnos_nuevos);
    int alumnos_total = (alumnos_orig + alumnos_nuevos);

    float *temp = realloc(notas, alumnos_total * sizeof(float));

        if (temp == NULL) {
            printf("Error de reserva de memoria.");
            free(notas);
            getchar();
            return -1;
        }

    notas = temp;

    for (int i = alumnos; i < alumnos_total; i++) {

        printf("Nota del alumno %d: ", i+1);
        int testNota2 = leer_float("", &notas[i]);

        if (testNota2 != 0) {
            printf("Error de lectura en alumno %d. Saliendo.", i+1);
            free(notas);
            getchar();
            return -1;
        }

        suma = suma + notas[i];
    }

    media = suma / alumnos_total;

    printf("La media nueva es: %.2f", media);

    free(notas);
    notas = NULL;

    getchar(); // pausa la consola hasta presionar enter
    return 0;
}