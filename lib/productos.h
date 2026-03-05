#ifndef PRODUCTOS_H
#define PRODUCTOS_H

#include <stdio.h>

static void imprimir_producto(const Producto prod);

static void listar_productos (const Producto *arr, int n);

static void alta_producto (Producto *arr, int *n, int max);

int buscar_indice_por_id (Producto *arr, int n, int id);

#endif