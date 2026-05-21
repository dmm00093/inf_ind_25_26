#include <stdio.h>
#include <stdlib.h>
#include "lectura.h"
#include <string.h>
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
// De cara a futuro: implementar cargar_texto con strtok_r, no necesario, pero mas seguro, buen ejercicio.

int cargar_texto(const char *ruta, Producto *arr, int cap, int *out_n)
{
    FILE *f = fopen(ruta, "r"); // Abrir fichero, estandar.
    if (!f) return 1; // Si no se puede abrir pues devolvemos un 1. Esto significa ERROR (para main).

    char linea [256]; // Creamos linea para guardar el array de 256 chars max.
    int n = 0; // n es el límite de parámetros, es decir, será 4 al final ya que son 4 parámetros en struct.

    while (n < cap && fgets(linea, sizeof(linea), f))
    {
        // if para quitar \n al final.
        size_t L = strlen(linea);
        if (L && linea[L-1] == '\n')
        {
            linea[L-1] = '\0';
        }

        // Ahora parseamos con sscanf. Como no editamos nada y le decimos exactamente como guardar
        // Pues no pasa nada. Ya que no hay posibilidad de error... nosotros no guardamos manualmente.
        // No tenemos que usar ninguna funcion extravagante.

        Producto pTemp; // Creamos un producto temporal por si sscanf falla.

        // ptemp puede ser: ptemp = {2, "Coca-Cola", 1.50, 5}. Si no falla, bien, si falla, pues, nos ahorramos meter basura
        // Y una vez tenemos eso, dentro de mi array de productos pues metemos esa p en mi posicion arr[n], que tiene n++
        // Entonces pues copiamos ese array temporal ahi dentro
        // p es un molde reutilizable que se rellena y se copia al array.

        if (sscanf(linea, "%d;%31[^;];%f;%d", &pTemp.id, pTemp.nombre, &pTemp.precio, &pTemp.stock) == 4)
        {
            arr[n++] = pTemp;
        }
    } fclose(f); // Cerramos fichero.

    *out_n = n;
    return 0;
}

int cargar_binario(const char *ruta_input, char *ruta_output)
{
    // ruta input es predefinida. ruta output es dinamica, nos dira donde está para que el programa lo use.
    FILE *f = fopen(ruta_input, "rb");

    if (!f) {
        //No se pudo cargar ruta.
        return 1;
    }

    fread(ruta_output, sizeof(char), 256, f); // leemos ruta salida

    fclose(f);

    return 0;
}

int guardar_texto(const char *ruta, const Producto *arr, int n)
{
    FILE *f = fopen(ruta, "w");
    if (!f) {
        perror("ERROR. Motivo de fallo: ");
        return 1;
    }

    for (int i = 0 ; i < n; i++) {
        fprintf(f, "%d;%s;%.2f;%d\n",
                arr[i].id, arr[i].nombre, arr[i].precio, arr[i].stock);
    }

    fclose(f);
    return 0;
}

int guardar_binario(const char *ruta_input, char *ruta_output)
{
    FILE *f = fopen(ruta_input, "wb"); // apertura en binario write

    if (!f) {
        printf("No se pudo cargar. "); perror("Error:");
        return 1;
    }

    fwrite(ruta_output, sizeof(char), 256, f); // igual que cargar con fwrite y wb.

    fclose(f);
    return 0;
}
/********************************************/

// Cuerpo de Programa //
/********************************************/
int main()
{
    // CARGADO DE PRODUCTOS Y CONFIGURACIÓN.

    SetConsoleOutputCP(65001); // Pasamos a UTF-8

    Producto maquina [MAXPRODS] = {0}; // iniciamos el array a {0} para evitar errores.
    int nProds = 0;

    // Iniciamos configuracion antes de cargar menu
    char ruta_config [256] = "../cfg/config.bin"; // Alocamos ruta del config para cambiarla a nuestro gusto.
    char ruta_config_out [256] = {0}; // Array sin basura, vacio con solo \0.

    printf(BLUE"/**************************************************/\n\n"RESET);
    printf(YELLOW"Iniciando Programa.\n\n"RESET);
    printf("Cargando configuracion...");

    if (cargar_binario(ruta_config, ruta_config_out) == 0) {
        printf(GREEN"\nCargado exitoso.\n"RESET);
        printf("\nRuta actual: "BLUE"%s\n\n"RESET, ruta_config_out);
    }
    else printf(RED"\nNo se encontro ruta productos."RESET" Realice mantenimiento.\n\n");

    printf("Presione"YELLOW" ENTER para continuar...\n\n"RESET);

    printf(BLUE"/**************************************************/\n"RESET);

    getchar(); // esperamos a enter
    system("cls"); // system manda una orden para que el cmd escriba lo que tu le dices en parentesis.
                           // cls -> clear screen clasico de siempre

    // PÁGINA PRINCIPAL //

    int opcionStart;

    do {

        printf("Página de inicio.\n\n");
        printf("1. Entrar a modo de compra\n");
        printf("2. Entrar a modo mantenimiento\n");
        printf("3. Ver configuración actual\n");
        printf("0. Salir\n\n");

        int testStart1 = leer_entero("Introduzca su opción: ", &opcionStart);

        while (testStart1 != 0) {
            printf("Error: opción inválida");
            testStart1 = leer_entero("Introduzca su opción: ", &opcionStart);
        }

        // TODO: Modo de compra

        switch (opcionStart) {

            case 1: { // MENU COMPRA

                if (cargar_binario(ruta_config, ruta_config_out) != 0 &&
                    cargar_texto(ruta_config_out, maquina, MAXPRODS, &nProds) == 0)
                {
                    printf(RED"\nError: no hay fichero de configuración. Realice mantenimiento\n"RESET);
                    printf("Presione" YELLOW " ENTER" RESET " para continuar.");
                    getchar();
                    system("cls");
                    break;
                }

                if (cargar_binario(ruta_config, ruta_config_out) == 0 &&
                                    cargar_texto(ruta_config_out, maquina, MAXPRODS, &nProds) != 0)
                {
                    printf(RED"\nError: no hay fichero de productos. Realice mantenimiento\n"RESET);
                    printf("Presione" YELLOW " ENTER" RESET " para continuar.");
                    getchar();
                    system("cls");
                    break;
                }

                if (cargar_binario(ruta_config, ruta_config_out) != 0 &&
                    cargar_texto(ruta_config_out, maquina, MAXPRODS, &nProds) != 0)
                {
                    printf(RED"\nError: no hay fichero de configuración. Realice mantenimiento\n");
                    printf("Error (2): no hay fichero de productos. Realice mantenimiento\n\n"RESET);
                    printf("Presione" YELLOW " ENTER" RESET " para continuar.");
                    getchar();
                    system("cls");
                    break;
                }

                if (cargar_binario(ruta_config, ruta_config_out) != 0 &&
                    cargar_texto(ruta_config_out, maquina, MAXPRODS, &nProds) != 0)
                {
                    printf("wip");
                    system("cls");
                    break;
                }
            }

            case 2: { // MENU MANTENIMIENTO

                system("cls");

                // MODO MANTENIMIENTO //

                int opcionMenu;

                do {

                    printf(".--------------------------.\n");
                    printf("|   " YELLOW " Modo Mantenimiento" RESET "   |\n");
                    printf(".--------------------------.\n");
                    printf("| "MAGENTA"1 - Guardar Productos"RESET"    |\n");
                    printf("| "MAGENTA"2 - Cargar Productos"RESET"     |\n");
                    printf("| "MAGENTA"3 - Listar Productos"RESET"     |\n");
                    printf("| "MAGENTA"4 - Anadir Productos"RESET"     |\n");
                    printf("| "MAGENTA"5 - Modificar Productos"RESET"  |\n");
                    printf("| "MAGENTA"6 - Buscar Producto"RESET"      |\n");
                    printf("| "GREEN"7 - Cargar Configuracion"RESET" |\n");
                    printf("| "GREEN"8 - Crear Configuracion"RESET"  |\n");
                    printf("| "GREEN"9 - Editar Configuracion"RESET" |\n");
                    printf("| "GREEN"10 - Ver Configuracion   "RESET"|\n");
                    printf("| "RED"0 - Salir a menú"RESET"                |\n");
                    printf(".--------------------------.\n");

                    int opcionTest = leer_entero("\nIntroduzca su opcion: ", &opcionMenu);

                    while (opcionTest != 0) {
                        printf("\nOpcion invalida.\n");
                        opcionTest = leer_entero("\nIntroduzca su opcion: ", &opcionMenu);
                    }

                    switch (opcionMenu)
                    {
                        case 1: { // Guardado Productos
                            printf("\nGuardando productos...\n\n"RED);

                            if (guardar_texto(ruta_config_out, maquina, nProds) == 0) {
                                printf(GREEN"Guardado exitoso.\n"RESET);
                            }
                            else printf("Error de guardado.\n\n"RESET);
                            break;
                        }

                        case 2: { // Carga productos
                            printf("\nCargando productos...\n\n");
                            if (cargar_texto(ruta_config_out, maquina, MAXPRODS, &nProds) == 0) {
                                printf(GREEN"Cargado exitoso\n"RESET);
                            }
                            else printf(RED"Error de carga.\n\n"RESET);
                            break;
                        }

                        case 3: {
                            printf("\nListando productos:\n");
                            if (nProds == 0) {
                                printf(RED"Error: No hay productos\n\n"RESET);
                                break;
                            }
                            listar_productos(maquina, nProds);
                            printf("\nListado terminado.\n");
                            break;
                        }

                        case 4: { // Adicionado productos
                            int opcion2;

                            int testOp2 = leer_entero("\nCuantos productos deseas adicionar: ", &opcion2);

                            if (testOp2 != 0 || opcion2 + nProds > MAXPRODS)
                            {
                                // Si se sale de la capacidad de la maquina, no podremos. O si es invalida entrada.
                                printf("No valido.\n");
                                    if (testOp2 != 0 ) {
                                        printf(RED "Error: introduzca un dato válido" RESET);
                                    }
                                    else {
                                        printf(RED"Error: no hay espacio en la máquina"RESET);
                                    }
                                return -1;
                            }

                            if (opcion2 == 0) {
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

                        case 5: { // Editado productos
                            int opcion3;

                            if (nProds == 0) {
                                printf(RED"Error: No hay productos\n"RESET);
                                break;
                            }

                            int testOp3 = leer_entero("\nIntroduzca la ID del producto a modificar: ", &opcion3);

                            if (testOp3 != 0) {
                                printf(RED"Error: entrada no valida.\n"RESET);
                                return -1;
                            }

                            modificar_producto(maquina, &nProds, opcion3);
                            break;
                        }

                        case 6: { // Busqueda producto
                            int opcion4;

                            if (nProds == 0) {
                                printf(RED"Error: No hay productos\n"RESET);
                                break;
                            }

                            int testop4 = leer_entero("\nBusque la ID deseada: ", &opcion4);

                            if (testop4 != 0) {
                                printf(RED"Error: entrada no valida.\n"RESET);
                                return -1;
                            }

                            buscar_indice_por_id(maquina, nProds, opcion4);
                            break;
                        }

                        case 7: {
                            printf("\nCargando config.bin...\n");

                            if (cargar_binario(ruta_config, ruta_config_out) == 0) {
                                printf(GREEN"\nCargado exitoso.\n"RESET);
                                printf("Ruta actual: "); printf(BLUE"%s\n"RESET, ruta_config_out);
                            }

                            else
                            {
                                printf(RED"\nRuta no encontrada."RESET" Especifique ruta de productos.\n\n");
                            }
                            break;
                        }

                        case 8: {

                            printf("\nCreador de configuracion\n\n");
                            // Debe especificar donde esta productos.txt, sin " ".
                            printf("Ejemplo al escribir ruta:" BLUE " ruta/ruta2/data/productos.txt\n" RESET);
                            printf(RED"No se deben incluir los caracteres "" al escribir la ruta.\n"RESET);

                            int testRuta = leer_cadena("Ruta de productos deseada: ", ruta_config_out, 256);

                            while (testRuta != 0) {
                                printf(RED"Error: entrada no valida.\n"RESET);
                                testRuta = leer_cadena("Ruta de productos deseada: ", ruta_config_out, 256);
                            }

                            if (guardar_binario(ruta_config, ruta_config_out) == 0) {
                                printf(GREEN"\nGuardado exitoso.\n"RESET);
                                printf("Ruta actual: "); printf(BLUE"%s\n"RESET, ruta_config_out);
                            }

                            else {
                                printf(RED"\nError en guardado. Saliendo..."RESET);
                            }
                            break;
                        }

                        case 9: {

                            printf("\nEditor de configuracion\n");

                            if (cargar_binario(ruta_config, ruta_config_out) != 0)
                            {
                                printf(RED"\nRuta no encontrada."RESET" Especifique ruta de productos.\n\n");
                                break;
                            }

                            int testRuta = leer_cadena("Ruta de productos deseada: ", ruta_config_out, 256);
                            while (testRuta != 0) {
                                printf(RED"Error: no valido.\n"RESET);
                                testRuta = leer_cadena("Ruta de productos deseada: ", ruta_config_out, 256);
                            }

                            if (guardar_binario(ruta_config, ruta_config_out) == 0) {
                                printf(GREEN"\nGuardado exitoso.\n"RESET);
                                printf("Ruta actual: "); printf(BLUE"%s\n"RESET, ruta_config_out);
                            }

                            else {
                                printf(RED"\nError en guardado. Saliendo..."RESET);
                            }

                            break;
                        }

                        case 10: {
                            if (cargar_binario(ruta_config, ruta_config_out) == 0) {
                                printf("Ruta actual: "); printf(BLUE"%s\n"RESET, ruta_config_out);
                            }

                            else
                            {
                                printf(RED"\nRuta no encontrada."RESET" Especifique ruta de productos.\n\n");
                            }

                            break;
                        }

                        case 0: {

                            printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();

                            system("cls");
                            break;
                        }

                        default: printf("\nNo valido. Saliendo...");
                    }

                }while (opcionMenu != 0);

                break; // Necesario para evitar que haga el siguiente case automatico

            } // } final case de opcion 2.

            case 3: {
                printf("\nRuta de configuración: " BLUE "%s" RESET, ruta_config);
                printf("\nRuta de productos: " MAGENTA "%s\n" RESET, ruta_config_out);
                printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                getchar();
                system("cls");
                break;
            }

            case 0 : {
                exit (1);
            }

            default: printf(RED"\nError: no valido.");
        }

    }while (opcionStart != 0);

    return 0;
}