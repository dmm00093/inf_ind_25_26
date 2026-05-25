#ifndef PUERTOSERIE_H
#define PUERTOSERIE_H

#include <stdlib.h>   // standard lib, para exit(1) u otros.
#include <stdio.h>    // standard input teclado.
#include <windows.h>  // Necesario para lo relacionado a puerto serie.
#include <stdint.h>   // Para uint...
#include <setupapi.h> // Para listar puertos
#include <devguid.h>  // Para listar puertos.
#include "lectura.h"  // Para pedir datos al usuario de forma segura.

// CLION -> EDIT CONFIGURATIONS -> EMULATE TERMINAL IN THE OUTPUT CONSOLE
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define BLACK   "\033[0;30m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"
// Si no, no funcionan los colores

// Colores de fondos de pantalla

#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

HANDLE openSerial(const char * puerto, uint32_t baud_rate);

int write_port(HANDLE port, uint8_t * buffer, size_t size);

SSIZE_T read_port(HANDLE port, uint8_t * buffer, size_t size);

void ListarPuertosSerie (); // Esta funcion lista los puertos disponibles usando setupAPI.

int PedirPuertoSeriei (); // esa i al final significa que es un int.

int PedirOpcioni (); // // esa i al final significa que es un int.

#endif