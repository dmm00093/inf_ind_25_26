#include "lectura.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int leer_entero (const char *input, int *output)
{
    char buf[128];

    printf("%s", input);

    fgets(buf, sizeof(buf), stdin);

    size_t len = strlen(buf);

    if (len > 0 && buf[len-1] == '\n')
    {
        buf [len-1] = '\0'; // quitamos \n al final de la cadena.
    }

    for (int i = 0 ; buf[i] != '\0' ; i++)
    {
        if ((!isdigit((unsigned char)buf[i])) && (!isspace((unsigned char)buf[i])))
            // si algun miembro no es un digito, salimos, no funciona.
            // ademas evitamos que salte error si hay un espacio al final.
            // isspace comprueba si un caracter es un espacio en blanco.
        {
            printf("Entrada no valida.\n");
            return -1;
        }
    }

    errno = 0;

    long valor = strtol (buf, NULL, 10);

    *output = (int) valor; // es como scanf("", &x). *output se comportara como x, y hay que poner &.

    return 0; // esto es lo que DEVUELVE la funcion, un 0 si funciona. en el main se comprueba.

} // Leemos entero con fgets y strtol. output 0 -> BIEN.

int leer_float (const char *input, float *output)
{
    char buf[128]; // Igual que antes.

    printf ("%s", input);

    fgets(buf, sizeof(buf), stdin);
    size_t len = strlen (buf);

        if (len > 0 && buf[len-1] == '\n')
        {
            buf [len-1] = '\0'; // Igual que antes.
        }

    errno = 0; // Igual.

    char *finptr;
    float val = strtof (buf, &finptr);

    if (errno != 0 && *finptr != '\0') //  - errno != 0 → número demasiado grande o pequeño
                                      //  - *finptr != '\0' → hay basura después del número
    {
        printf("Entrada no valida.");
        return -1;
    }

    /*Tan pronto como encuentra un carácter que no puede interpretar
    (por ejemplo una letra que no sea parte de la notación científica) → se detiene*/

    *output = val;
    return 0;

} // Leemos float con fgets y luego strtof.

int leer_cadena (const char *input, char *output, size_t max_len)
{
    printf ("%s", input); fgets (output, max_len, stdin); // output ya es algo que entra a la func.

    size_t len = strlen (output);

        if (len > 0 && output[len-1] == '\n')
        {
            output[len-1] = '\0';
        }

    return 0;
} // Leemos un string.