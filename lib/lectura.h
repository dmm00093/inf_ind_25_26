#ifndef LECTURA_H // Necesario empezar con if not defined y terminar el end if al final
#define LECTURA_H

#include <stddef.h>  // para size_t
#include <stdio.h>   // para FILE, printf, fgets
#include <stdlib.h>  // para strtol, strtof
#include <ctype.h>   // para isdigit, isspace

int leer_entero (const char *input, int *output); // lectura entero con fgets y strtol

int leer_float (const char *input, float *output); // Leemos float con fgets y luego strtof.

int leer_cadena (const char *input, char *output, size_t max_len); // Leemos un string.

#endif