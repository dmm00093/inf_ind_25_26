#include "ficheros.h"

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
        // p es  reutilizable, se rellena y se copia al array.

        if (sscanf(linea, "%d;%31[^;];%f;%d", &pTemp.id, pTemp.nombre, &pTemp.precio, &pTemp.stock) == 4)
        {
            arr[n++] = pTemp;
        }
		
    } fclose(f); // Cerramos fichero.

    *out_n = n;
    return 0;
}

int cargar_binario(const char *ruta_input, Configuracion *ruta_output) // Usamos el struct para el output
{
    // ruta input es predefinida. ruta output es dinamica, nos dira donde está para que el programa lo use.
    FILE *f = fopen(ruta_input, "rb");

    if (!f) {
        //No se pudo cargar ruta.
        return -1;
    }

    fread(ruta_output, sizeof(Configuracion), 1, f); // leemos ruta salida de tamaño del struct.
													 // 1->cantidad de elementos, que será solo 1. 	

	// Si usaramos un char, vale claro. Hacemos 256, porque introducimos 256 chars de 1 byte, que son, 256 bytes, guay.
	// Pero como usaremos un struct, no queremos meter 256 structs. Eso es inviable, la memoria, kaput.

    fclose(f);

    return 0;
}

int guardar_texto(const char *ruta, const Producto *arr, int n)
{
    FILE *f = fopen(ruta, "w");
    if (!f) {
        perror("ERROR. Motivo de fallo: ");
        return -1;
    }

    for (int i = 0 ; i < n; i++) {
        fprintf(f, "%d;%s;%.2f;%d\n",
                arr[i].id, arr[i].nombre, arr[i].precio, arr[i].stock);
    }

    fclose(f);
    return 0;
}

int guardar_binario(const char *ruta_input, Configuracion *ruta_output)
{
    FILE *f = fopen(ruta_input, "wb"); // apertura en binario write

    if (!f) {
        printf("No se pudo cargar. "); perror("Error:");
        return -1;
    }

    fwrite(ruta_output, sizeof(Configuracion), 1, f); // igual que cargar, pero con fwrite y wb.

    fclose(f);
    return 0;
}