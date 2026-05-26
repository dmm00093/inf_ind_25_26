#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <string.h> // Necesario para strlen

// Definiremos el struct de productos aquí, para luego no tener que ir definiendo structs en el main
// Esto no es que sea muy intuitivo pero para lo que nosotros queremos simplifica mucho el main...

typedef struct {
    int id;
    char nombre[32]; // Asumo este tamaño por tu "%31[^;]" en sscanf
    float precio;
    int stock;
} Producto;

// Definiremos un struct de CONFIGURACION aquí. Ya que tenemos config de la maquina y del STM.
// Así me ahorro configOK y blabla veinte veces y hacer un cargar_binario,texto otras 20 veces.

#pragma pack (push, 1) // <-- Esto es Gemini. El compilador puede meterme bytes de relleno y joderlo todo.
typedef struct {
    char ruta_productos [256];
    char puertoCOM [20];
	int puertoCOMdisplay;
    int baudios;
	int configOK; // Esto es algo local. En vez de definir un int en mi main veremos este int en el struct.
				  // La idea no fue mia, fue por gemini. Porque me estaba volviendo loco haciendo todo otra vez en el main.
				  // configOK es 0 -> OK todo 
				  // configOK es .1 -> kaput
} Configuracion;
#pragma pack(pop)

// 2. Funciones
int cargar_texto(const char *ruta, Producto *arr, int cap, int *out_n);
int cargar_binario(const char *ruta_input, Configuracion *ruta_output);
int guardar_texto(const char *ruta, const Producto *arr, int n);
int guardar_binario(const char *ruta_input, Configuracion *ruta_output);

#endif // ARCHIVOS_H