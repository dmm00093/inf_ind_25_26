#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"


/*
* Parte A: Menu mínimo + lectura robusta
* Implementar leer_entero , leer_float , leer_cadena
* Menú con switch usando leer_entero
*/

int leer_entero (const char *input, int *output)
{
    char buf[128]; // buffer de 128 va de sobra. valid es para ver si es valido (opcional)

    printf("%s", input); // es como scanf, nuestra funcion. esto es para escribir "escribe blabla..."

    fgets(buf, sizeof(buf), stdin); //fgets lee en buffer un standard input

    // fgets -> buf (donde guardamos la cadena) , sizeof(buf) es MAXIMO caracteres a leer, stdin es input (teclado)

    size_t len = strlen(buf);
    // la longitud de la cadena en fgets sera len, que usa strlen para meterlo en su size_t
    // NO CUENTA hasta el \0 al final.

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

    errno = 0; // restablecemos numero de errores si la cadena es OK. variable GLOBAL.
               // se usa para reportar overflow o underflow

    long valor = strtol (buf, NULL, 10); // strtol tiene tres argumentos:

    // long strtol(const char *nptr, char **endptr, int base);

    /* nptr -> la cadena de texto que quieres convertir (buf)
       endptr → opcional, te dice dónde terminó la conversión.
       Por ejemplo: si buf = "123abc", *endptr apuntará a "abc". Si pones NULL → convierte todo lo que pueda
       base → base numérica, normalmente 10 para decimal
    */

    *output = (int) valor; // es como scanf("", &x). *output se comportara como x, y hay que poner &.
    return 0; // esto es lo que DEVUELVE la funcion, un 0 si funciona. en el main se comprueba.

} // Leemos entero con fgets y strtol. output 0 -> BIEN.
                                                      // output =/= 0 -> MAL.

int leer_float (const char *input, float *output)
{
    char buf[128]; // Igual que antes.

    printf ("%s", input);

    fgets(buf, sizeof(buf), stdin); size_t len = strlen (buf);

        if (len > 0 && buf[len-1] == '\n')
        {
            buf [len-1] = '\0'; // Igual que antes.
        }

    errno = 0; // Igual.

    char *finptr;
    float val = strtof (buf, &finptr); // Por que usamos finptr?

    // ESTRUCTURA STRTOF: float strtof(const char *nptr, char **endptr):
    /*
    nptr es basicamente la longitud de la cadena, lo que quieres convertir.
    char endptr te dice DONDE TERMINÓ la conversión.
    usar endptr nos permite DETECTAR el error, o basura. si usamos NULL, solo convierte e ignora resto.
    por ejemplo, con 123.45abc, endptr detecta abc, sabes que hay basura. NULL le daria igual :)
    */

    if (errno !=0 && *finptr != '\0') //  - errno != 0 → número demasiado grande o pequeño
                                      //  - *finptr != '\0' → hay basura después del número
    {
        printf("Entrada no valida.");
        return -1;
    }

    /*Tan pronto como encuentra un carácter que no puede interpretar
    (por ejemplo una letra que no sea parte de la notación científica) → se detiene*/

    *output = val; return 0;
} // Leemos float con fgets y luego strtof.

int leer_cadena (const char *input, char *output, size_t max_len)
{
    printf ("%s", input); fgets (output, max_len, stdin); // output ya es algo que entra a la func.

    /*
 * Explicación de output:
 * - output es un punter que ya viene desde main.
 * - La función escribe directamente en ese buffer usando fgets.
 * - No es necesario “devolver” output; la cadena queda disponible en el buffer de main.
 * - Cuidado: si usáramos un buffer interno local, habría que copiarlo a output antes de salir.
 * - Al final, quitamos el salto de línea '\n' que fgets deja al final de la cadena.
 */

    size_t len = strlen (output);

        if (len > 0 && output[len-1] == '\n')
        {
            output[len-1] = '\0';
        }

    return 0;
} // Leemos un string.

int main()
{
    int opcion;

    do {
        printf("\n+--------------------------+\n");
        printf("|        " YELLOW "   MENU           " RESET "|\n");
        printf("+--------------------------+\n");
        printf("| "MAGENTA"1 - Numero entero"RESET"        |\n");
        printf("| "MAGENTA"2 - Numero real"RESET"          |\n");
        printf("| "MAGENTA"3 - Cadena de caracteres"RESET" |\n");
        printf("| "RED"0 - Salir"RESET"                |\n");
        printf("+--------------------------+\n");

        int testOp = leer_entero("Introduzca su opcion: ", &opcion);

            if (testOp != 0)
            {
                printf("Abandonando...\n");
                return -1;
            }

        switch (opcion)
        {

            case 0:
            {
                printf("Abandonando programa.\n");
                break;
            }

            case 1:
            {
                int num; // nuestro numero a guardar
                int test = leer_entero("Introduzca un entero: ", &num); // aqui vemos si es 0 (OK) o 1 (RIP)

                if (test == 0)
                {
                    printf("Tu entero es: %d\n", num); // OK
                }

                break;
            }

            case 2:
            {
                float numB;
                int testB = leer_float("\nIntroduzca un numero real: ", &numB);

                if (testB == 0)
                {
                    printf("Tu numero real es: %.2f\n", numB);
                }

                break;
            }

            case 3:
            {
                char strA [128];
                int testC = leer_cadena("\nIntroduzca un string: ", strA, sizeof(strA));

                if (testC == 0)
                {
                    printf("Tu string es: %s\n", strA);
                }

                break;
            }

            default: printf("No valido."); break;
        }

    } while (opcion != 0);

    return 0;

}// Cuerpo de la funcion
