#include <stdio.h>
#include <stdlib.h>

typedef struct { // Hay que iniciar structs, primero de todo.
    int id;
    char nombre [32];
    float precio;
    int stock;
} Producto; // Nombre del struct

static void imprimir_producto(const Producto prod) // ver main, definimos prod como subproductos
{
    printf("ID: %d | %-20s | %.2f EUR | Stock: %d\n", prod.id, prod.nombre, prod.precio, prod.stock);
}

static void listar_productos (const Producto *arr, int n) // hay que pasar todo a un array. ver main
{
    printf("\n--- PRODUCTOS ---\n");

    for (int i = 0; i < n; i++)
    {
        imprimir_producto (arr[i]); // vemos cada subproducto en el array definido en main (maquina, por ej)
    }
}

int main ()
{
    // Definimos todos los productos.

    Producto prod1 = {
        .id = 1,
        .nombre = "Coca-Cola",
        .precio = 2.0,
        .stock = 15
    };

    Producto prod2 = {
        .id = 2,
        .nombre = "Pepsi",
        .precio = 2.00f,
        .stock = 12
    };

    Producto prod3 = {
        .id = 3,
        .nombre = "Snickers",
        .precio = 1.50f,
        .stock = 20
    };

    Producto prod4 = {
        .id = 4,
        .nombre = "Chicles",
        .precio = 0.75f,
        .stock = 30
    };

    Producto prod5 = {
        .id = 5,
        .nombre = "Agua 500ml",
        .precio = 1.25f,
        .stock = 25
    };prod1

    // Creamos array con lista de todos los productos.

    Producto maquina[5] = {prod1, prod2, prod3, prod4, prod5}; // necesario para la funcion, almacenamos todo.

    listar_productos(maquina, 5); // con ese array mostramos los productos con la funcion.
}