#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include "lectura.h"
#include <windows.h> // LIBRERIA COLORES

// CLION -> EDIT CONFIGURATIONS -> EMULATE TERMINAL IN THE OUTPUT CONSOLE
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"
// Si no, no funcionan los colores

// Struct //
/***********************************/
#define MAXPRODS 15 // Maximo Productos

typedef struct { // Hay que iniciar structs, primeramente
    int id;
    char nombre [32];
    float precio;
    int stock;
} Producto; // Nombre del struct
/********************************************/

// Funciones Productos //
/********************************************/
static void imprimir_producto(const Producto prod) // ver main, definimos prod como subproductos
{
    printf("ID: %d | %-20s | %.2f EUR | Stock: %d\n", prod.id, prod.nombre, prod.precio, prod.stock);
    // Basicamente imprimimos los productos que estan contenidos en ese array de Producto.
}

static void listar_productos (const Producto *arr, int n) // hay que pasar tódo a un array. ver main
{
    printf("\n|------------------- PRODUCTOS -------------------|\n");

    for (int i = 0; i < n; i++)
    {
        imprimir_producto (arr[i]); // vemos cada subproducto en el array definido en main (maquina, por ej)
    }
    printf("|-------------------------------------------------|\n");

}

static void alta_producto (Producto *arr, int *n, int max)
{
    // Comprobamos si hay espacio:

    if (*n >= max)
    {
        printf("Error: maquina llena.");
        return;
    }

        Producto nuevo; // Creamos un nuevo struct temporal que ira al array.

        // Preguntamos especificaciones.

        int testId = leer_entero("ID nueva: ", &nuevo.id);
        if (testId != 0) {
            printf("Error: no valido.\n");
            return;
        }

        int testNom = leer_cadena("Nombre nuevo: ", nuevo.nombre, 32);
        if (testNom != 0) {
            printf("Error: no valido.\n");
            return;
        }

        int testPrc = leer_float("Precio nuevo: ", &nuevo.precio);
        if (testPrc != 0) {
            printf("Error: no valido.\n");
            return;
        }

        int testStk = leer_entero("Stock nuevo: ", &nuevo.stock);
        if (testStk != 0) {
            printf("Error: no valido.\n");
            return;
        }

        arr[*n] = nuevo; // Ahora, en esa posicion libre del array, metemos nuestro struct que es ahora permanente.
        (*n)++; // Incrementamos el numero de productos ya que hemos metido uno nuevo.
}

int buscar_indice_por_id (Producto *arr, int n, int id)
{
    for (int i = 0; i < n; i++) {
        if (arr[i].id == id) {
            printf("La [ID: %d] corresponde al indice [%d] del producto: [%s].\n",
                          arr[i].id, i, arr[i].nombre);
            return i;
        }
    }

    printf("No se encuentra.\n");
    return -1;
}

static void modificar_producto (Producto *arr, int *n, int id)
{
    for (int i = 0 ; i < *n ; i++) {
        if (arr[i].id == id)
        {
            printf("\nProducto seleccionado:\n");

            printf("[%s]: ID(%d), Precio(%.2f EUR), Stock(%d ud).\n\n", arr[i].nombre, arr[i].id, arr[i].precio, arr[i].stock);

            int idTest = leer_entero("Introduzca la nueva ID: ", &arr[i].id);
            if (idTest != 0) {
                printf("Error: Entrada invalida.");
                return;
            }

            int nomTest = leer_cadena("Introduzca el nuevo nombre: ", arr[i].nombre, 32);
            if (nomTest != 0) {
                printf("Error: Entrada invalida.");
                return;
            }

            int prTest = leer_float("Introduzca el nuevo precio: ", &arr[i].precio);
            if (prTest != 0) {
                printf("Error: Entrada invalida.");
                return;
            }

            int stkTest = leer_entero("Introduzca el nuevo stock: ", &arr[i].stock);
            if (stkTest != 0) {
                printf("Error: Entrada invalida.");
                return;
            }

            return;
        }
    }
}
/********************************************/

// Funciones Ficheros //
/********************************************/
int cargar_texto(const char *ruta, Producto *arr, int cap, int *out_n)
{
    FILE *f = fopen(ruta, "r");
    if (!f) return 1;

    char linea [256]; int n = 0;

    while (n < cap && fgets(linea, sizeof(linea), f))
    {
        // if para quitar \n al final.
        size_t L = strlen(linea);
        if (L && linea[L-1] == '\n')
        {
            linea[L-1] = '\0';
        }

        // Leemos la cadena entera. Usaremos strtok_r porque sscanf es malo.

        char *auxptr; // Puntero auxiliar para que strtok sepa por donde va.
        char *token = strtok_r(linea, ";", &auxptr); // Primer token, es decir.

        // "15;Choco;83.23;24" -> primer token es 15. Para en ;. auxptr le dira en el siguiente caso que va por ahi.
        // Luego, la siguiente llamada, tiene NULL como primer arg, porque ya no necesita abrir nada. Entonces sigue
        // Asi hasta que llegue a \0, es decir, NULL.

        int opcion = 0;
        while (token != NULL) // Hasta que termine la linea.
        {
            switch (opcion)
            {
                case 0: arr[n].id = atoi (token); // Convertimos esa parte del array a un int, float, str...
                    break;                  // El token pasa a ser parte del struct.
                case 1: strcpy(arr[n].nombre, token);
                    break;
                case 2: arr[n].precio = atof (token);
                    break;
                case 3: arr[n].stock = atoi (token);
                    break;
                default: printf("Error: fallo de lectura.");
                    return -1;
            }
            opcion++;
            token = strtok_r (NULL, ";", &auxptr);
        }

        if (opcion !=4)
        {
            printf("Error: linea incompleta");
            return -1;
        }
        n++;
    }

    fclose(f);

    *out_n = n;
    return 0;
}

int guardar_texto(const char *ruta, const Producto *arr, int n)
{
    FILE *f = fopen(ruta, "w");
    if (!f) return 1;

    for (int i = 0 ; i < n; i++) {
        fprintf(f, "%d;%s;%.2f;%d\n",
                arr[i].id, arr[i].nombre, arr[i].precio, arr[i].stock);
    }

    fclose(f);
    return 0;
}
/********************************************/

int main()
{
    Producto prod0 = {
        .id = 0,
        .nombre = "Coca-Cola",
        .precio = 2.0,
        .stock = 15
    };

    Producto prod1 = {
        .id = 1,
        .nombre = "Pepsi",
        .precio = 2.00f,
        .stock = 12
    };

    Producto prod2 = {
        .id = 2,
        .nombre = "Snickers",
        .precio = 1.50f,
        .stock = 20
    };

    Producto prod3 = {
        .id = 3,
        .nombre = "Chicles",
        .precio = 0.75f,
        .stock = 30
    };

    Producto prod4 = {
        .id = 4,
        .nombre = "Agua 500ml",
        .precio = 1.25f,
        .stock = 25
    };

    int nProds = 5;
    Producto maquina [MAXPRODS] = {prod0, prod1, prod2, prod3, prod4};

    int opcionMenu;
    do
    {

        printf("\n.--------------------------.\n");
        printf("|   " YELLOW " MAQUINA EXPENDEDORA" RESET "   |\n");
        printf(".--------------------------.\n");
        printf("| "MAGENTA"1 - Guardar Productos"RESET"    |\n");
        printf("| "MAGENTA"2 - Cargar Productos"RESET"     |\n");
        printf("| "MAGENTA"3 - Listar Productos"RESET"     |\n");
        printf("| "MAGENTA"4 - Anadir Productos"RESET"     |\n");
        printf("| "MAGENTA"5 - Modificar Productos"RESET"  |\n");
        printf("| "MAGENTA"6 - Buscar Producto"RESET"      |\n");
        printf("| "RED"0 - Salir"RESET"                |\n");
        printf(".--------------------------.\n");

        int opcionTest = leer_entero("\nIntroduzca su opcion: ", &opcionMenu);

        if (opcionTest != 0) {
            printf("\nOpcion invalida. Saliendo...\n");
            system("pause");
            break;
        }

        switch (opcionMenu)
        {

            case 1: {
                printf("\nGuardando productos...\n");

                if (guardar_texto("../tema6-ficheros/data/productos.txt", maquina, nProds) == 0) {
                    printf("Guardado exitoso.");
                }
                else printf("Error de guardado.");
                break;
            }

            case 2: {
                int nprods2;
                printf("\nCargando productos...\n");
                if (cargar_texto("data/productos.txt", maquina, 256, &nprods2) == 0) {
                    printf("Cargado exitoso");
                }
                else printf("Error de carga.");
                break;
            }

            case 3: {
                printf("\nListando productos:\n");
                listar_productos(maquina, nProds);
                printf("\nListado terminado.\n");
                break;
            }

            case 4: {
                int opcion2;
                int testOp2 = leer_entero("\nCuantos productos deseas adicionar: ", &opcion2);

                if (testOp2 != 0 || opcion2 + nProds > MAXPRODS)
                {
                    printf("Error: no valido.\n");
                    return -1;
                }

                else if (opcion2 == 0) {
                    printf("Abandonando menu...");
                }

                for (int i = 1 ; i <= opcion2; i++)
                {
                    printf("\nEspecifique su producto numero: %d\n", i);
                    alta_producto(maquina, &nProds, MAXPRODS); // Definimos MAXPRODS al inicio como constante. Es una maquina.

                    // &nProds ya que usamos paso por referencia y vamos a editar su valor directamente en memoria, no copiamos.
                }

                break;
            }

            case 5: {
                int opcion3;
                int testOp3 = leer_entero("\nIntroduzca la ID del producto a modificar: ", &opcion3);

                if (testOp3 != 0) {
                    printf("Error: no valido.\n");
                    return -1;
                }

                modificar_producto(maquina, &nProds, opcion3);
                break;
            }

            case 6: {

                int opcion4;
                int testop4 = leer_entero("\nBusque la ID deseada: ", &opcion4);

                if (testop4 != 0) {
                    printf("Error: no valido.\n");
                    return -1;
                }

                buscar_indice_por_id(maquina, nProds, opcion4);
                break;
            }

            case 0: {
                printf("Abandonando programa.\n");
                break;
            }

            default: printf("\nNo valido. Saliendo...");
        }

    }while (opcionMenu != 0);

    return 0;
}