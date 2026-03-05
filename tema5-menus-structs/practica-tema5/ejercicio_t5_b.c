#include <stdio.h>
#include "lectura.h"
#define MAXPRODS 15

typedef struct { // Hay que iniciar structs, primeramente
    int id;
    char nombre [32];
    float precio;
    int stock;
} Producto; // Nombre del struct

// El struct es como un molde. Es la base de cualquier producto, que tendrá estas 4 cosas. No crea nada.

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
    // Premisa de funcion: pasamos un array (desreferenciado), un int (paso por referencia) y el maximo de prods.
    // El int, se pasa por referencia. Al pasar por referencia, al terminar la funcion, va a ser MODIFICADO
    // De forma permanente. De forma que si le metemos 5 y se convierte en 6, esa variable introducida pasara a
    // Ser 6 de forma efectiva. Si no lo pasamos por referencia (quitamos *), solo va a COPIAR esa variable.
    // Pero no la va a editar directamente en memoria.

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

int main ()
{
    // Definimos todos los productos.

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

    // Creamos array con lista de todos los productos.

    int nProds = 5;

    Producto maquina[MAXPRODS] = {prod0, prod1, prod2, prod3, prod4}; // necesario para la funcion, almacenamos everything.
    // maquina es un array de structs Producto. Cada elemento (struct) almacena su propia id, nombre, precio, stock...

    listar_productos(maquina, nProds); // con ese array mostramos los productos con la funcion.

    int opcionB;
    int testopB = leer_entero("\nBusque la ID que desee: ", &opcionB);

    if (testopB != 0) {
        printf("Error: no valido.");
        return -1;
    }

    buscar_indice_por_id(maquina, nProds, opcionB);

    int opcion;
    int testop = leer_entero("\nDesea adicionar productos?\n1) Continuar\n0) Salir\n-->", &opcion);

        if (testop != 0) {
            printf("Error: no valido.");
            return -1;
        }
        else if (opcion == 0) {
            printf("Abandonando programa...");
            return 0;
        }

    int numAdd;
    int testAdd = leer_entero("Cuantos productos deseas adicionar: ", &numAdd);

        if (testAdd != 0 || numAdd + nProds > MAXPRODS)
        {
            printf("Error: no valido.\n");
            return -1;
        }

        else if (numAdd == 0) {
            printf("Abandonando menu...");
        }

    for (int i = 1 ; i <= numAdd; i++)
    {
        printf("Especifique su producto n: %d\n", i);
        alta_producto(maquina, &nProds, MAXPRODS); // Definimos MAXPRODS al inicio como constante. Es una maquina.

        // &nProds ya que usamos paso por referencia y vamos a editar su valor directamente en memoria, no copiamos.
    }

    printf(".......................\nMostrando nuevo listado\n.......................");

    listar_productos(maquina, nProds); // volvemos a listar.
}