#include "productos.h"
#include <stdio.h>

// Producto DEBE ser definido o la libreria no funcionara.

static void imprimir_producto(const Producto prod) // definimos prod como subproductos
{
    printf("ID: %d | %-20s | %.2f EUR | Stock: %d\n", prod.id, prod.nombre, prod.precio, prod.stock);
}

static void listar_productos (const Producto *arr, int n)
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
    // Premisa de funcion: pasamos un array (desreferenciado), un int (paso por referencia) y el maximo de prods.
    // El int, se pasa por referencia. Al pasar por referencia, al terminar la funcion, va a ser MODIFICADO
    // De forma permanente. De forma que si le metemos 5 y se convierte en 6, esa variable introducida pasara a
    // Ser 6 de forma efectiva. Si no lo pasamos por referencia (quitamos *), solo va a COPIAR esa variable.
    // Pero no la va a editar directamente en memoria.

    if (*n >= max)
    {
        printf("Error: maquina llena.");
        return;
    }

        Producto nuevo;

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

        arr[*n] = nuevo;
        (*n)++;
}

int buscar_indice_por_id (Producto *arr, int n, int id)
{
    for (int i = 0; i <= n; i++) {
        if (arr[i].id == id) {
            printf("La [ID: %d] corresponde al indice [%d] del producto: [%s].\n",
                          arr[i].id, i, arr[i].nombre);
            return id;
        }

    }
    printf("No se encuentra.");
    return -1;
}