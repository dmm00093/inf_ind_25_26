#include <stdio.h>
#include <stdlib.h>
#include "lectura.h"
#include "productos.h"

int main()
{
    int opcionMenu;

    do
    {
        printf("\n+--------------------------+\n");
        printf("|       MAQUINA EXPENDEDORA |\n");
        printf("+--------------------------+\n");
        printf("| 1 - Listar Productos      |\n");
        printf("| 2 - Añadir Productos      |\n");
        printf("| 3 - Modificar Productos   |\n");
        printf("| 4 - Buscar Producto       |\n");
        printf("| 0 - Salir                 |\n");
        printf("+--------------------------+\n");
    }while (opcionMenu != 0);

}