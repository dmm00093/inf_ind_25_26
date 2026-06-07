/* Nombre: Mexp_STM_Final
 * Autor: David Martinez (dmm00093@red.ujaen.es)
 * Asignatura: Informatica Industrial
 * Proyecto Ordinaria 2: Maquina Expendedora + Puerto Serie + STM32
 * Funcion de programa: Interfaz de maquina expendedora en Windows CMD + Integración PC-STM32 + Sincronía PC-STM32
*/

/* Nota: todas las librerias necesarias (stdio, windows, setupapi, devguid, etc, están INCLUIDAS en las definiciones de los include
 * Es decir, puertoserie.h, ficheros.h, lectura.h, ya las incluyen. puertoserie.h incluye todas, es algo redundante, pero:
 * si se quieren usar librerias diferentes sin usar la otra, es necesario. 
 * Nota: puertoserie.h si que utiliza funciones de ficheros.h, son codependientes.
*/

#include <stdlib.h>   // standard lib, para exit(1) u otros.
#include "ficheros.h" // ficheros
#include "lectura.h"  // Para pedir datos al usuario de forma segura.
#include "puertoserie.h" // Libreria funciones pserie. 

// ---------------------------------------------------------------------------------------------------------------------
//             ADVERTENCIA: CERRAR CUALQUIER PROGRAMA QUE PUEDA USAR EL COM DE STLINK (CUBEIDE, ETC) Y
//             NECESARIO  : CARGAR "PuertoSerie_USART_PC.c" O CUALQUIER OTRO PROGRAMA SÍMIL EN EL STM32.
// ---------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------
//             ADVERTENCIA: STRUCT DE MAQUINA, CONFIG GESTIONADO EN EL HEADER DE LA LIBRERIA FICHEROS.
//             NECESARIO  : CARGAR LIBRERIA SI SE VAN A USAR LOS STRUCTS Y FUNCIONES ASOCIADAS
// ---------------------------------------------------------------------------------------------------------------------

// Funciones Productos //
/********************************************/
static void imprimir_producto(const Producto prod) { // ver main, definimos prod como subproductos

    printf(CYAN "  ID: " YELLOW "%d" RESET " | " GREEN "%-20s" RESET " | " MAGENTA "%.2f EUR" RESET " | " BLUE "Stock: %d" RESET "\n", prod.id, prod.nombre, prod.precio, prod.stock);
    // Basicamente imprimimos los productos que estan contenidos en ese array de Producto.

    // Colores y puesto "bonito" por Gemini, es perder el tiempo lo contrario para este tipo de cosas.
}

static void listar_productos(const Producto *arr, int n) { // hay que pasar tódo a un array. ver main
    printf(BOLD "\n" BG_BLUE "                    Escoja su aperitivo                  " RESET "\n\n");

    for (int i = 0; i < n; i++) {
        imprimir_producto(arr[i]); // vemos cada subproducto en el array definido en main (maquina, por ej)
    }
    printf(BOLD "\n" BG_BLUE "                                                         " RESET "\n\n");
}

static void alta_producto(Producto *arr, int *n, int max) {

    // Comprobamos si hay espacio:

    if (*n >= max) {
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

int buscar_indice_por_id(Producto *arr, int n, int id) {
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

static void modificar_producto(Producto *arr, int *n, int id) {
    for (int i = 0; i < *n; i++) {
        if (arr[i].id == id) {
            printf("\nProducto seleccionado:\n");

            printf("[%s]: ID(%d), Precio(%.2f EUR), Stock(%d ud).\n\n", arr[i].nombre, arr[i].id, arr[i].precio,
                   arr[i].stock);

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

/*
********************************************
             Cuerpo de Programa
********************************************
*/

#define MAXPRODS 15 // Maximo Productos de Máquina

int main() {

    // CONFIGURACION VARIABLES PARA STM32 //

    // TODO: Tema de comunicacion PC-STM y lo del boton, monedas...
    // TODO: Refinar interfaz, colores en modo compra.

    // CARGADO DE PRODUCTOS Y CONFIGURACIÓN.

    SetConsoleOutputCP(65001); // Pasamos a UTF-8

    Producto maquina[MAXPRODS] = {0}; // iniciamos el array a {0} para evitar errores.
    int nProds = 0;

    Configuracion config = {0}; // creamos array del struct. iniciamos en 0, si no, mal asunto (memoria basura).
    config.configOK = -1; // de primeras es -1.

	/* Cosas a considerar del struct:
	 * 1. NO tenemos ruta_config_out. Ahora tenemos config.ruta_productos.
	 * 2. Si queremos acceder al struct habrá que usar &config o el nombre que le demos.
	*/

    // Iniciamos configuracion antes de cargar menu
    char ruta_config[256] = "../cfg/config.bin"; // Alocamos ruta del config para cambiarla a nuestro gusto.

    int testConfigOK = cargar_binario(ruta_config, &config); // La funcion devuelve un 0 si ok
                                                             // Tira la info a &config, y accede a el, si no, mal asunto.
                                                             // Asi sabe donde esta el struct.
    int prodsOK = -1; // Por defecto mal
    int modo1ok;

    printf(BLUE"••••••••••••••••••••••••••••••••••••••••••••••••••••••\n\n"RESET);

    printf(YELLOW"Iniciando Programa.\n\n"RESET);
    printf("Cargando configuracion...\n");

    if (testConfigOK == 0) {
        config.configOK = 0;
        prodsOK = cargar_texto(config.ruta_productos, maquina, MAXPRODS, &nProds); // Devuelve 0 si OK
    }
    if (config.configOK == 0 && prodsOK == 0) {
        printf(GREEN"\nCargado exitoso.\n"RESET);
        printf(YELLOW"\n••) "RESET BLUE"Configuración de Máquina" YELLOW" (••\n"RESET);
        printf(YELLOW"\n•) "GREEN"Ruta de configuración: " BLUE "%s" RESET, ruta_config);
        printf(YELLOW"\n•) "GREEN"Ruta de productos    : " BLUE "%s\n" RESET, config.ruta_productos);
        printf("\n" RESET BLUE "Los productos están cargados. "RESET"\n\n");
        modo1ok = 0;
    } else if (config.configOK == 0 && prodsOK != 0) {
        printf(BG_RED BOLD"\nERROR: No se encontraron productos. Realice mantenimiento.\n" RESET);
        modo1ok = -1;
    } else if (config.configOK != 0) {
        printf(BG_RED BOLD"\nERROR: No se encontró configuración. Realice mantenimiento.\n" RESET);
        modo1ok = -1;
    }

    // Comprobamos si el usuario ha definido el STM32.
    if (config.baudios == 0 || config.puertoCOM [0] == '\0'){
        printf(BG_RED BOLD "\nERROR: STM32 no configurado.\n" RESET);
    } else {
        printf(YELLOW"••) "RESET MAGENTA "Configuración de STM32" YELLOW" (••\n"RESET);
        printf(YELLOW"\n•) "RESET GREEN "Puerto Serie actual   : " MAGENTA "COM%d\n"RESET, config.puertoCOMdisplay);
        printf(YELLOW"•) " GREEN "Tasa de baudios actual: " MAGENTA "%d\n"RESET, config.baudios);
    }

    printf("\nPresione"YELLOW" ENTER " RESET "para continuar...\n\n"RESET);

    printf(BLUE"••••••••••••••••••••••••••••••••••••••••••••••••••••••\n\n"RESET);

    getchar(); // esperamos a enter
    system("cls"); // system manda una orden para que el cmd escriba lo que tu le dices en parentesis.
    // cls -> clear screen clasico de siempre

    // PÁGINA PRINCIPAL //

    int opcionStart;

    do {

        // Usé Gemini para hacer el menú "bonito". Es perder el tiempo lo contrario (06/06...)

        // Borde superior
        printf(RESET CYAN "╔══════════════════════════════════════════════════╗\n" RESET);
        printf(CYAN "║" BLUE "                 PÁGINA DE INICIO                 " RESET CYAN "║\n" RESET);
        printf(CYAN "╠══════════════════════════════════════════════════╣\n" RESET);
        printf(CYAN "║                                                  ║\n" RESET);

        // Opciones del menú
        printf(CYAN "║  " YELLOW BOLD "[1]" RESET BOLD " Entrar a modo de compra                    " CYAN " ║\n" RESET);
        printf(CYAN "║  " YELLOW BOLD "[2]" RESET BOLD " Gestión de la máquina (mantenimiento)      " CYAN " ║\n" RESET);
        printf(CYAN "║  " YELLOW BOLD "[3]" RESET BOLD " Gestión de hardware (STM32)                " CYAN " ║\n" RESET);
        printf(CYAN "║  " YELLOW BOLD "[4]" RESET BOLD " Ver configuración actual                   " CYAN " ║\n" RESET);

        printf(CYAN "║                                                  ║\n" RESET);

        // Opción de salir (En Rojo)
        printf(CYAN "║  " RED BOLD "[0]" RESET BOLD " Salir                                      " CYAN " ║\n" RESET);

        // Borde inferior
        printf(CYAN "╚══════════════════════════════════════════════════╝\n\n" RESET);

        int testStart1 = leer_entero(GREEN BOLD "Introduzca su opción: " RESET, &opcionStart);

        while (testStart1 != 0) {
            printf("\nError: opción inválida\n\n");
            testStart1 = leer_entero("Introduzca su opción: ", &opcionStart);
        }

        system("cls");

        // TODO: Modo de compra

        switch (opcionStart) {
            case 1: {
                // MENU COMPRA

                if (modo1ok == -1) { // No dejamos entrar sin configurar
                    printf(BG_RED BOLD"\nError: Realice mantenimiento\n"RESET);
                    printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                    getchar();
                    system("cls");
                    break;
                }

                if (modo1ok == 0) { // si ok pues, adelante

                    // CLION -> MANDA IMPORTE Y PRECIO DEL PRODUCTO.
                    // CLION VA A CONVERTIR LOS FLOATS A UINT16_T (maximo de 100 euros, 14 bits)
                    // MANDAMOS UINT16_T AL STM32 Y EL LO PROCESA Y DEVUELVE.

                    float saldoCLION; // Importe introducido en float.
                    float carritoCLION; // Carrito en float.

                    uint16_t saldoSTM32; // Importe introducido en uint
                    uint16_t carritoSTM32; // Carrito en uint.

                    listar_productos(maquina, nProds);

                    int selProd;
                    int prodEncontrado = -1; // Testigo para decir si ha encontrado lo que quiere el usuario, o no.

                    do {
                        // Buscamos el producto que se quiere
                        int testProd = leer_entero(CYAN "\nSELECCIONE PRODUCTO: " RESET, &selProd);

                        while (testProd != 0) {
                            printf(BG_RED "\nOpción inválida, vuelva a intentarlo.\n" RESET);
                            testProd = leer_entero(CYAN "SELECCIONE PRODUCTO:" RESET, &selProd);
                        }

                        for (int i = 0; i < nProds; i++) {
                            if (selProd == maquina[i].id) {
                                prodEncontrado = i;
                                break;
                            }
                        }

                        if (prodEncontrado == -1) {
                            printf(
                                BG_RED BOLD "\nNo se encuentra el producto." RESET BG_RED " Intentelo de nuevo\n"
                                RESET);
                        }
                    } while (prodEncontrado == -1);

                    printf(GREEN "\nPRODUCTO SELECCIONADO: " YELLOW "%s\n" RESET, maquina[prodEncontrado].nombre);

                    carritoCLION = maquina[prodEncontrado].precio;
                    printf(MAGENTA "\nIMPORTE A PAGAR: " YELLOW "%.2f\n" RESET, carritoCLION);

                    int testImporte = leer_float(BLUE "\nINTRODUZCA IMPORTE: " RESET YELLOW, &saldoCLION);

                    while (testImporte != 0) {
                        printf(BG_RED "\nOpción inválida, vuelva a intentarlo.\n" RESET);
                        testImporte = leer_float(BLUE "SELECCIONE PRODUCTO:" RESET YELLOW, &saldoCLION);
                    }

                    printf(RESET "" RESET);

                    if (saldoCLION > 100.0f) {
                        // Maximo de 100 euros admisibles (10.000 centimos).
                        printf(RED "\nImporte máximo de 100 EUR." RESET);
                        while (saldoCLION > 100.0f || testImporte != 0) {
                            printf(BG_RED "\nOpción inválida, vuelva a intentarlo.\n" RESET);
                            testImporte = leer_float(CYAN "SELECCIONE PRODUCTO:" RESET, &saldoCLION);
                        }
                    }

                    // CONVERTIMOS A UINT16_T. El buffer del stm es suficientemente grande.

                    carritoSTM32 = (uint16_t) ((carritoCLION * 100.0f) + 0.5f);
                    saldoSTM32 = (uint16_t) ((saldoCLION * 100.0f) + 0.5f);

                    /* (uint16_t) -> por leches forzamos que sea un entero de 16 bits.
                     * carritoCLION * 100.0f -> pasamos a centimos y forzamos que sea en float no en double.
                     * le sumamos 0.5float es decir 0.5 en formato float no formato double, que es 64bit.
                     * ese 0.5 es para que no tenga errores al truncar un 149.9999 a 149 centimos y no 150 cents.
                     * el pc procesa esto de sobra... el problema es el stm que no, pero los enteros se los come
                     * ...diria yo
                     */

                    // Antes de mandar: PROCESAR los numeros trocitos a trocitos para que el STM lo pueda leer bien.
                    // Ahorro de recursos.

                    char mensajeSTM[20]; // Creo un string para los numeros

                    // si carrito es 150 y saldo es 100 -> mandamos -> 150\n100\n (sabemos cuando empieza y termina).
                    sprintf(mensajeSTM, "%05u\n%05u\n", carritoSTM32, saldoSTM32); // mandamos

                    // Quiero hacer que mis numeros llegen en cinco cifras. Por ejemplo, 00862\n00953.
                    // Asi puedo hacer una funcion mas directa para procesar. Lo de 05u lo desconocia -> Gemini.

                    // Enviamos
                    // B1 USER: the user button is connected to the I/O PC13 (pin 2) of the STM32 microcontroller.

                    uint8_t buff[100] = {0}; // Buffer lleno de ceros.
                    HANDLE port = openSerial(config.puertoCOM, config.baudios); // Abrimos conexión.

                    read_port(port, buff, sizeof(buff));
                    // Intenta leer si el STM32 había enviado algo viejo mientras se conectaba.
                    // Como el buffer inicial buff se sobrescribe, esto básicamente limpia la línea.

                    write_port(port, mensajeSTM, strlen(mensajeSTM)); // Envía texto.

                    FlushFileBuffers(port); // Aseguramos que mande el mensaje entero.

                    // DISPLAY EN CONSOLA //

                    read_port(port, buff, sizeof(buff));
                    printf(buff);

                    // Escucha respuesta de STM32. Saca el mensaje de la bandeja de COMx.

                    // Respuesta de maquina a traves del stm

                    printf(BG_BLUE BOLD "               Resultado de su compra               " RESET);

                    if (buff[0] == '-') {
                        printf(BG_RED "\n\n[ERROR] OPERACIÓN RECHAZADA\n" RESET);
                        printf(RED "Detalle: Importe insuficiente o máquina sin cambio.\n\n" RESET);

                    } else if (buff[0] == '0') {

                        printf(BG_GREEN "\n\n[OK]" RESET GREEN" COMPRA ACEPTADA CON ÉXITO\n\n" RESET);

                    } else {
                        printf(YELLOW "\n\nRespuesta en bruto del STM32:\n" RESET);
                        printf("%s\n", buff);
                    }
                    printf(BG_BLUE "                                                    " RESET);

                    CloseHandle(port); // Cerramos el puerto.
                    FlushFileBuffers(port); // Aseguramos que mande el mensaje entero.

                    getchar();
                    system("cls");
                    break;
                }
            }

            case 2: {

                // MENU MANTENIMIENTO

                system("cls");

                printf("Bienvenido al asistente de configuración de máquina.\n\n");
                int opcionMenu;

                do {
                    printf(".--------------------------.\n");
                    printf("|   " YELLOW " Gestión de Máquina" RESET "    |\n");
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
                    printf("| "RED"0 - Salir a menú"RESET"         |\n");
                    printf(".--------------------------.\n");

                    int opcionTest = leer_entero("\nIntroduzca su opcion: ", &opcionMenu);

                    while (opcionTest != 0) {
                        printf("\nOpcion invalida.\n");
                        opcionTest = leer_entero("\nIntroduzca su opcion: ", &opcionMenu);
                    }

                    switch (opcionMenu) {
                        case 1: {
                            // Guardado Productos
                            printf("\nGuardando productos...\n\n"RED);

                            if (guardar_texto(config.ruta_productos, maquina, nProds) == 0) {
                                printf(GREEN"Guardado exitoso.\n"RESET);
                            } else printf("Error de guardado.\n\n"RESET);

							printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 2: {
                            // Carga productos
                            printf("\nCargando productos...\n\n");
                            if (cargar_texto(config.ruta_productos, maquina, MAXPRODS, &nProds) == 0) {
                                printf(GREEN"Cargado exitoso\n"RESET);
                            } else printf(RED"Error de carga.\n\n"RESET);

							printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();
                            system("cls");

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

							printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 4: {
                            // Adicionado productos
                            int opcion2;

                            int testOp2 = leer_entero("\nCuantos productos deseas adicionar: ", &opcion2);

                            if (testOp2 != 0 || opcion2 + nProds > MAXPRODS) {
                                // Si se sale de la capacidad de la maquina, no podremos. O si es invalida entrada.
                                printf("No valido.\n");
                                if (testOp2 != 0) {
                                    printf(RED "Error: introduzca un dato válido" RESET);
                                } else {
                                    printf(RED"Error: no hay espacio en la máquina"RESET);
                                }
                                return -1;
                            }

                            if (opcion2 == 0) {
                                printf("\nNo se ha especificado. Saliendo...\n");
								printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
								getchar();
								system("cls");
                            }

                            for (int i = 1; i <= opcion2; i++) {
                                printf("\nEspecifique su producto numero: %d\n", i);
                                alta_producto(maquina, &nProds, MAXPRODS);
                                // Definimos MAXPRODS al inicio como constante. Es una maquina.

                                // &nProds ya que usamos paso por referencia y vamos a editar su valor directamente en memoria, no copiamos.
                            }

							printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 5: {
                            // Editado productos
                            int opcion3;

                            if (nProds == 0) {
                                printf(RED"Error: No hay productos\n"RESET);

								printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
								getchar();
								system("cls");
                                break;

                            }

                            int testOp3 = leer_entero("\nIntroduzca la ID del producto a modificar: ", &opcion3);

                            if (testOp3 != 0) {

                                printf(RED"Error: entrada no valida.\n"RESET);

                                printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
								getchar();
								system("cls");
                            }

                            modificar_producto(maquina, &nProds, opcion3);

							printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 6: {
                            // Busqueda producto
                            int opcion4;

                            if (nProds == 0) {
                                printf(RED"Error: No hay productos\n"RESET);

								printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
								getchar();
								system("cls");

                                break;
                            }

                            int testop4 = leer_entero("\nBusque la ID deseada: ", &opcion4);

                            if (testop4 != 0) {
                                printf(RED"Error: entrada no valida.\n"RESET);
                                printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
								getchar();
								system("cls");
                            }

                            buscar_indice_por_id(maquina, nProds, opcion4);
                            break;
                        }

                        case 7: {
                            printf("\nCargando config.bin...\n");

                            if (cargar_binario(ruta_config, &config) == 0) {
                                printf(GREEN"\nCargado exitoso.\n"RESET);
                                printf("Ruta actual: ");
                                printf(BLUE"%s\n"RESET, config.ruta_productos);
                            } else {
                                printf(RED"\nRuta no encontrada."RESET" Especifique ruta de productos.\n\n");
                            }

							printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 8: {
                            printf("\nCreador de configuracion\n\n");
                            // Debe especificar donde esta productos.txt, sin " ".
                            printf("Especifique de ruta de productos.\n");
                            printf("Ejemplo al escribir ruta:" BLUE " ruta/ruta2/data/productos.txt\n" RESET);
                            printf(RED"No se deben incluir los caracteres """" al escribir la ruta.\n"RESET);

                            int testRuta = leer_cadena("Ruta de productos deseada: ", config.ruta_productos, 256);

                            while (testRuta != 0) {
                                printf(RED"Error: entrada no valida.\n"RESET);
                                testRuta = leer_cadena("Ruta de productos deseada: ", config.ruta_productos, 256);
                            }

                            if (guardar_binario(ruta_config, &config) == 0) {
                                printf(GREEN"\nGuardado exitoso.\n"RESET);
                                printf("Ruta actual: ");
                                printf(BLUE"%s\n"RESET, config.ruta_productos);
                            } else {
                                printf(RED"\nError en guardado. Saliendo."RESET);
								printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
								getchar();
								system("cls");
                            }

							printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 9: {
                            printf("\nEditor de configuracion\n");

                            if (cargar_binario(ruta_config, &config) != 0) {
                                printf(RED"\nRuta no encontrada."RESET" Especifique ruta de productos.\n\n");
								printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
								getchar();
								system("cls");
                                break;
                            }

                            int testRuta = leer_cadena("Ruta de productos deseada: ", config.ruta_productos, 256);
                            while (testRuta != 0) {
                                printf(RED"Error: no valido.\n"RESET);
                                testRuta = leer_cadena("Ruta de productos deseada: ", config.ruta_productos, 256);
                            }

                            if (guardar_binario(ruta_config, &config) == 0) {
                                printf(GREEN"\nGuardado exitoso.\n"RESET);
                                printf("Ruta actual: ");
                                printf(BLUE"%s\n"RESET, config.ruta_productos);
                            } else {
                                printf(RED"\nError en guardado. Saliendo."RESET);
                            }

							printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 10: {
                            if (cargar_binario(ruta_config, &config) == 0) {
                                printf("Ruta actual: ");
                                printf(BLUE"%s\n"RESET, config.ruta_productos);
                            } else {
                                printf(RED"\nRuta no encontrada."RESET" Especifique ruta de productos.\n\n");
                            }

							printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 0: {
                            system("cls");
                            break;
                        }

                        default: printf("\nNo valido. Saliendo...");
                    }
                } while (opcionMenu != 0);

                break; // Necesario para evitar que haga el siguiente case automatico
            } // } final case de opcion 2.

            case 3: {

                int opcionMenu;
                printf(RESET BG_MAGENTA BOLD "Bienvenido al asistente de gestión de hardware\n\n" RESET);

                do {

                    printf(MAGENTA BOLD"1. Elegir puerto serie (comunicación)\n");
                    printf("2. Elegir tasa de baudios (comunicación)\n");
                    printf("3. Guardar configuración\n");
                    printf("4. Cargar configuración\n");
                    printf("5. Ver configuración\n");
                    printf(RED"0. Salir\n"RESET);

                    int opcionTest = leer_entero("\nIntroduzca su opcion: ", &opcionMenu);

                    while (opcionTest != 0) {
                        printf(BG_RED BOLD "\nError: Opcion invalida.\n" RESET);
                        opcionTest = leer_entero("\nIntroduzca su opcion: ", &opcionMenu);
                    }
                    switch (opcionMenu) {

                        case 1: {

                            // LISTADO Y ELECCIÓN DE PUERTOS SERIE //

                            printf("\n");

                            int errorListar = ListarPuertosSeriei(); // Listamos. En la función se usa su propio SetConsoleOutput.
                            // La funcion la programe para devolver un int que si es 0 sale error. Esto lo usamos
                            // por si tenemos que usar break y no queremos que siga haciendo el recorrido de pedir puert.

                            if (errorListar == 0) { // Si no hay puertos --> adieu
                                system("cls");
                                break;
                            }

                            SetConsoleOutputCP(65001); // Pasamos a UTF-8 despues de usar la funcion con ANSI.

                            int PuertoUsuario = PedirPuertoSeriei();
                            config.puertoCOMdisplay = PuertoUsuario;

                            printf(RESET"\nHa escogido el puerto "BLUE"(COM%d).\n\n"RESET, PuertoUsuario);

                            sprintf(config.puertoCOM, "\\\\.\\COM%d", PuertoUsuario);
                            // Guardamos el puerto en el struct config.


                            printf("Presione" YELLOW " ENTER " RESET "para continuar.");
                            getchar();
                            system("cls");

                            break;

                            /* Gemini: En C y C++, la barra invertida \ es un carácter de escape.
                             * \\.\ es por el Win32 Device Namespace. Es algo que han inventado para hablar estrictamente de Hardware.
                             * Como ya no estamos en MS-DOS, han tenido que crearlo, ya que MS-DOS iba de 0 a 9 COMs.
                             * Si tenemos un COM10, 11... no funcionaría con COM%d. Hay que usar \\.\
                             * Entonces, queda esa cosa tan larga debido a que para una \ hay que escribir \\.
                            */
                        }

                        case 2: {

                            // ELECCIÓN DE BAUDIOS //

                            printf("\nEscoja la opción de tasa de baudios para la comunicación.\n\n");

                            printf(BLUE"1"RESET" -> 1200\n"
                                BLUE"2"RESET" -> 2400\n"
                                BLUE"3"RESET" -> 4800\n"
                                BLUE"4"RESET" -> 9600\n"
                                BLUE"5"RESET" -> 19200\n"
                                BLUE"6"RESET" -> 38400\n"
                                BLUE"7"RESET" -> 57600\n"
                                BLUE"8"RESET" -> 115200\n");

                            int BaudiosUsuarioOpcion = PedirOpcioni();
                            int BaudiosUsuarioTasa = 0;

                            if (BaudiosUsuarioOpcion == 1) {
                                BaudiosUsuarioTasa = 1200;
                            } else if (BaudiosUsuarioOpcion == 2) {
                                BaudiosUsuarioTasa = 2400;
                            } else if (BaudiosUsuarioOpcion == 3) {
                                BaudiosUsuarioTasa = 4800;
                            } else if (BaudiosUsuarioOpcion == 4) {
                                BaudiosUsuarioTasa = 9600;
                            } else if (BaudiosUsuarioOpcion == 5) {
                                BaudiosUsuarioTasa = 19200;
                            } else if (BaudiosUsuarioOpcion == 6) {
                                BaudiosUsuarioTasa = 38400;
                            } else if (BaudiosUsuarioOpcion == 7) {
                                BaudiosUsuarioTasa = 57600;
                            } else if (BaudiosUsuarioOpcion == 8) {
                                BaudiosUsuarioTasa = 115200;
                            } else {
                                // Control de errores por si introducen un número fuera del rango 1-8
                                printf(RED"\nOpción inválida."RESET" Asignando 9600 baudios por defecto.\n");
                                BaudiosUsuarioTasa = 9600;
                            }

                            printf("\nHa escogido la tasa de" BLUE" %d baudios.\n\n"RESET, BaudiosUsuarioTasa);
							config.baudios = BaudiosUsuarioTasa;

                            printf("Presione" YELLOW " ENTER " RESET "para continuar.");
                            getchar();
                            system("cls");
                            break;
                        }

                        case 3: {

							// Comprobamos si el usuario ha definido algo.
							if (config.baudios == 0 || config.puertoCOM [0] == '\0'){
								printf("\nError. Debe definir especificaciones\n\n");
							    printf("Presione" YELLOW " ENTER " RESET "para continuar.");
							    getchar();
							    system("cls");
								break;
							}

							system("cls");
                            printf(MAGENTA BOLD"\nGuardado de Configuración\n" RESET);

							// Realmente este menu no hace nada, pero esta bien para ver todo
							if (guardar_binario(ruta_config, &config) == 0) {
							    printf(BG_MAGENTA BOLD "\nConfiguración guardada exitosamente\n" RESET);
							    printf(RESET"\nHa escogido el puerto "BLUE"COM%d\n"RESET, config.puertoCOMdisplay);
							    printf("\nHa escogido la tasa de" BLUE" %d baudios.\n\n"RESET, config.baudios);
							} else {
                                printf(BG_RED BOLD "Error: No se pudo guardar la configuración\n");
                            }

							printf("Presione" YELLOW " ENTER " RESET "para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 4: {

                            system("cls");
                            printf("Cargando config.bin...\n");

                            if (cargar_binario(ruta_config, &config) == 0) {
                                printf(GREEN"\nCargado exitoso.\n"RESET);
                                printf("Ruta actual: ");
                                printf(BLUE"%s\n"RESET, ruta_config);
                            } else {
                                printf(RED"\nRuta no encontrada."RESET" Especifique ruta de configuración.\n\n");
                            }

							printf("Presione" YELLOW " ENTER " RESET "para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }

                        case 5: {

                            system("cls");

                            printf(RESET BG_MAGENTA BOLD "Configuración actual\n"RESET);
                            printf(YELLOW"\n•) "RESET GREEN "Ruta actual             : " MAGENTA "%s"RESET, ruta_config);
                            printf(YELLOW"\n•) "RESET GREEN "Puerto actual           : " MAGENTA "COM%d"RESET, config.puertoCOMdisplay);
                            printf(YELLOW"\n•) "RESET GREEN "Tasa de baudios actuales: " MAGENTA "%d"RESET, config.baudios);

                            printf("\n\nPresione" YELLOW " ENTER " RESET "para continuar.");
                            getchar();
                            system("cls");
                        }

                        case 0: {
                            system("cls");
                            break;
                        }

                        default: {
                            printf(BG_RED BOLD"\nError: no válido\n"RESET);

							printf("Presione" YELLOW " ENTER " RESET "para continuar.");
                            getchar();
                            system("cls");

                            break;
                        }
                    }
                }while (opcionMenu != 0);

                break;
            }

            case 4: {

                config.configOK = cargar_binario(ruta_config, &config); // igual que al start

                if (testConfigOK == 0) {
                    config.configOK = 0;
                    prodsOK = cargar_texto(config.ruta_productos, maquina, MAXPRODS, &nProds); // Devuelve 0 si OK
                }
                if (config.configOK == 0 && prodsOK == 0) {
                    printf(GREEN"[•••] Visualizador de configuración [•••]\n"RESET);
                    printf(YELLOW"\n••) "RESET BLUE"Configuración de Máquina" YELLOW" (••\n"RESET);
                    printf(YELLOW"\n•) "GREEN"Ruta de configuración: " BLUE "%s" RESET, ruta_config);
                    printf(YELLOW"\n•) "GREEN"Ruta de productos    : " BLUE "%s\n" RESET, config.ruta_productos);
                    printf("\n" RESET BLUE "Los productos están cargados. "RESET"\n\n");
                    modo1ok = 0;
                } else if (config.configOK == 0 && prodsOK != 0) {
                    printf(BG_RED BOLD"\nERROR: No se encontraron productos. Realice mantenimiento.\n" RESET);
                    modo1ok = -1;
                } else if (config.configOK != 0) {
                    printf(BG_RED BOLD"\nERROR: No se encontró configuración. Realice mantenimiento.\n" RESET);
                    modo1ok = -1;
                }

                // Comprobamos si el usuario ha definido el STM32.
                if (config.baudios == 0 || config.puertoCOM [0] == '\0'){
                    printf(BG_RED BOLD "\nERROR: STM32 no configurado.\n" RESET);
                } else {
                    printf(YELLOW"••) "RESET MAGENTA "Configuración de STM32" YELLOW" (••\n"RESET);
                    printf(YELLOW"\n•) "RESET GREEN "Puerto Serie actual   : " MAGENTA "COM%d\n"RESET, config.puertoCOMdisplay);
                    printf(YELLOW"•) " GREEN "Tasa de baudios actual: " MAGENTA "%d\n"RESET, config.baudios);
                }

                printf("\nPresione" YELLOW " ENTER" RESET " para continuar.");
				getchar();
                system("cls");

                break;
            }

            case 0: {
                exit(1);
            }

            default: {

				printf(BG_RED BOLD"\nError: no valido."RESET);

				printf("Presione" YELLOW " ENTER " RESET "para continuar.");
                getchar();
                system("cls");

				break;

			}
        }
    } while (opcionStart != 0);

    return 0;
}