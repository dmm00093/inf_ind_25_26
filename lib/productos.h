#ifndef PRODUCTOS_H
#define PRODUCTOS_H

#include <stdio.h>

// Definición del struct para ser usado, es requerido
typedef struct {
    int id;
    char nombre[32];
    float precio;
    int stock;
} Producto;

// Funciones de productos
void listar_productos(const Producto *arr, int n);
void alta_producto(Producto *arr, int *n, int max);
int buscar_indice_por_id(Producto *arr, int n, int id);
void modificar_producto(Producto *arr, int *n, int id);

// Funciones de lectura para que las pueda usar
int leer_entero(const char *mensaje, int *variable);
int leer_cadena(const char *mensaje, char *variable, int max_long);
int leer_float(const char *mensaje, float *variable);

#endif // PRODUCTOS_H