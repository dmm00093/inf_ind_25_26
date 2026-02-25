// Created by dmm00093 on 23/02/2026.

// 1) Completa el programa para que imprima:
#include <stdio.h>

int main(void) {
    int x = 5;
    int *p = &x;

    // A) Imprimir x, &x, p, *p

    printf("\nVariable y su direccion de memoria --> x = %d, dir(x) = %p\n", x, &x); // print de x y &
    printf("Puntero y su valor asociado --> p = %p, *p = %d\n", p, *p); // print de p y su valor asoc.

    // B) Modificar x a 99 con *p, C) Imprimir x. Apartados juntos

    printf("\nModificacion de x con *p:\n");
    *p = 99; printf("x = %d\n", x); // modif. de x con *p

    return 0;
}