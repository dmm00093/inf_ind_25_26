#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <string.h> // Necesario para strlen

// 1. Cremaos el struct productos en el .h para que se pueda usar
typedef struct {
    int id;
    char nombre[32]; // Asumo este tamaño por tu "%31[^;]" en sscanf
    float precio;
    int stock;
} Producto;

// 2. Creamos el struct de configuracion. Esto es mas versatil para todo

typedef struct {
    char ruta_productos;
    int puertoCOM;
    int baudios;
} Configuracion;

// 2. Funciones
int cargar_texto(const char *ruta, Producto *arr, int cap, int *out_n);
int cargar_binario(const char *ruta_input, char *ruta_output);
int guardar_texto(const char *ruta, const Producto *arr, int n);
int guardar_binario(const char *ruta_input, char *ruta_output);

#endif // ARCHIVOS_H