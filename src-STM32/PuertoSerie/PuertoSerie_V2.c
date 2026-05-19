/* Programa: PuertoSerie_v1
 * Editado por: dmm00093 (original dado por profesor)
 * Funcion:
 *      1. Ofrece un listado de puertos serie disponibles usando SetupAPI y DevGUID.
 *      2. Permite habilitar al usuario cualquier puerto serie deseado.
*/

#include <stdio.h>    // standard input teclado.
#include <windows.h>  // Necesario para lo relacionado a puerto serie.
#include <stdint.h>   // Para uint...
#include <setupapi.h> // Para listar puertos
#include <devguid.h>  // Para listar puertos.
#include "lectura.h"  // Para pedir datos al usuario de forma segura.

#define RED     "\033[1;31m" // Colores ansi.
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

// ---------------------------------------------------------------------------------------------------------------------
//             ADVERTENCIA: CERRAR CUALQUIER PROGRAMA QUE PUEDA USAR EL COM DE STLINK (CUBEIDE, ETC) Y
//             NECESARIO  : CARGAR "PuertoSerie_USART_PC.c" O CUALQUIER OTRO PROGRAMA SÍMIL EN EL STM32.
// ---------------------------------------------------------------------------------------------------------------------

HANDLE openSerial(const char * puerto, uint32_t baud_rate){ // Funcion de configuracion de puerto serie.

    HANDLE p = CreateFileA(puerto,GENERIC_READ | GENERIC_WRITE,0,
                           NULL,OPEN_EXISTING,0, NULL);
    // Decimos a Win que abra el archivo del puerto COM seleccionado
    // GENERIC_READ,WRITE -> Queremos transmitir y recibir (TX,RX).
    // Devuelve un HANDLE que es un identificador o puntero al puerto que está abierto.

    if(p == INVALID_HANDLE_VALUE) {
        printf("Error al crear el puerto\n");
        return INVALID_HANDLE_VALUE;
    }

    BOOL ok = FlushFileBuffers(p); // Limpiamos buffer de sesión anterior, inicio limpio.
    if(!ok){
        printf("Error al vaciar buffer\n");
        CloseHandle(p);
        return INVALID_HANDLE_VALUE;
    }

    COMMTIMEOUTS timeouts;
    // Le decimos a Win que no queremos que el programa cuelgue intentando leer. Es decir:
    // En el STM tenemos que while (...RXNE == 0), no haga nada, se congela hasta que reciba algo.
    // Aqui no. Intentaremos leer, pero si pasan 50 ms y no hay nada, seguimos adelante.

    GetCommTimeouts(p,&timeouts);
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 0;
    timeouts.ReadIntervalTimeout         = 0;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    ok = SetCommTimeouts(p,&timeouts);

    if(!ok){
        printf("Error al cargar timeouts\n");
        CloseHandle(p);
        return INVALID_HANDLE_VALUE;
    }

    DCB estado; // Espejo para el STM32.

    GetCommState(p,&estado);
    estado.BaudRate = baud_rate; // Configuramos baudios, debe ser igual que los del STM!
    estado.ByteSize = 8; // Tamaño del registro, 8 bytes (USART_DR es de 8 bytes).
    estado.Parity   = NOPARITY; // Sin paridad, no hemos programado eso aun.
    estado.StopBits = ONESTOPBIT; // Tiene un bit de parada, manual del STM.

    ok = SetCommState(p,&estado);

    if(!ok){
        printf("Error al cargar estado\n");
        CloseHandle(p);
        return INVALID_HANDLE_VALUE;
    }

    return p;
}

int write_port(HANDLE port, uint8_t * buffer, size_t size) { // Equivalente a USART2_EscribirString.

    DWORD written;
    BOOL ok = WriteFile(port, buffer, size, &written, NULL);
    // Aqui hacemos que tome lo que haya en el buffer, que mide size bytes, y la manda al port.

    if (!ok) {
        printf("Error al escribir puerto\n");
        return -1;
    }

    if (written != size) {
        printf("Error al escribir todos los bytes en el puerto\n");
        return -1;
    }
    return 0;
}

SSIZE_T read_port(HANDLE port, uint8_t * buffer, size_t size) {

    DWORD received;
    BOOL ok = ReadFile(port, buffer, size, &received, NULL);

    if (!ok){
        printf("Error al leer del puerto\n");
        return -1;
    }
    return received;
}

void ListarPuertosSerie () {

    // Listado puertos serie //

    SetConsoleOutputCP(1252);

    // Funcion:
    // (https://learn.microsoft.com/es-es/windows/win32/api/setupapi/nf-setupapi-setupdigetclassdevsw)
    // (https://learn.microsoft.com/es-es/windows/win32/api/setupapi/nf-setupapi-setupdigetdeviceregistrypropertya)
    // (https://learn.microsoft.com/es-es/windows/win32/api/setupapi/nf-setupapi-setupdienumdeviceinfo)

    // Usamos las librerias setupapi y la libreria devguid (las usa el admin de disps) para obtener nombres de COM.
    // Importante saber que tenemos que linkear la libreria setupapi en CMake para que funcione.
    // Usamos un bucle while (GetClassDevs) con un if dentro (RegProp (la llamaremos asi, obtener nombre puerto)).
    // Segun lo que devuelva la funcion if (TRUE o FALSE) sabremos si hay error.
    // En caso de error, saldrá del bucle while y pondrá nuestro código de error. Si no, continúa.

    uint8_t bufferSetup [5000]; // RegProp necesita un dato tipo PBYTE. Es más seguro usar uint8_t.
                                // Microsoft los inventó hace mucho pero causan fugas de memoria y no son portables.
    // https://stackoverflow.com/questions/74401584/about-the-windows-byte-and-pbytes-data-types.

    SP_DEVINFO_DATA idDispositivo; // SP_DEVINFO_DATA es TODA la lista de puertos que hay en mi Windows.
                                   // idDispositivo es mi dispositivo individual que quiero encontrar
                                   // Luego, lo recorremos en el bucle while. Es un struct vacio con una variable.

    idDispositivo.cbSize = sizeof(SP_DEVINFO_DATA); // necesario por compatibilidad al parecer para evitar overflow.

    HDEVINFO idTemp = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS,NULL,NULL,
                 DIGCF_PRESENT); // DIGCF_PRESENT: Filtrado de SOLO los dispositivos activos

    // Handle to Device Information (cortesia gemini).
    // Windows guarda toda la lista gigante de dispositivos en una zona protegida de su memoria.
    // A cambio, te da un ticket (idTemp).
    // Cada vez que quieres consultar algo de esa lista, le enseñas el ticket a Windows

    int k = 0;
    int testError = 0;

    printf(GREEN"Mostrando puertos serie.\n\n"RESET);

    while ((SetupDiEnumDeviceInfo(idTemp, k, &idDispositivo) != 0)) {

        // [in]  HDEVINFO         DeviceInfoSet,
        // [in]  DWORD            MemberIndex,
        // [out] PSP_DEVINFO_DATA DeviceInfoData

        // Esta función rellena nuestro struct vacío con datos reales.
        // Tenemos que pasar &idDispositivo para que MODIFIQUE ese parámetro (puntero) -> PASO POR REFERENCIA.

        if (SetupDiGetDeviceRegistryPropertyA(idTemp, &idDispositivo, SPDRP_FRIENDLYNAME,
                      NULL, bufferSetup, sizeof(bufferSetup), NULL) != FALSE) {

            // SetupDiGetDeviceRegistryProperty devuelve TRUE si la llamada se realizó correctamente.
            // De lo contrario, devuelve FALSE, error registrado se puede recuperar mediante una llamada GetLastError.
            // SetupDiGetDeviceRegistryProperty devuelve el código de error ERROR_INVALID_DATA.
            // si la propiedad solicitada no existe para un dispositivo o si los datos de propiedad no son válidos.

            // Caso donde funcione la llamada:

            SetConsoleOutputCP(65001); // UTF-8. Necesario. Usan codificacion diferente... blegh.
            printf(YELLOW"Opción"); // Se verá bonito en la consola.

            SetConsoleOutputCP(1252); // La funcion devuelve datos codificados en ANSI Latin 1.
            printf(" %d -> "BLUE"%s\n"RESET, k, bufferSetup); // Se verá igual de bonito.
        }

        k++; // Incrementamos k para recorrer el siguiente puerto.
        testError = testError + k;
    }

    if (testError == 0) { // Caso de error, no hay puertos que mostrar o ha fallado la llamada a RegProps.

        SetConsoleOutputCP(65001); // UTF-8.

        printf(RED"Error al obtener los puertos disponibles. Compruebe su configuración.\n"RESET);
        printf("\nPresione"YELLOW" ENTER"RESET" para salir...");

        SetupDiDestroyDeviceInfoList(idTemp); // Debemos limpiar memoria antes de terminar, siempre.

        DWORD ErrorMsj = GetLastError(); // Obtenemos el error obtenido si es que ha habido error en RegProps.
                                         // Double Word, 4 bytes.

        if (ErrorMsj) {
            printf("Código de error de Windows: "RED" %lu\n"RESET, ErrorMsj); // %lu -> long unsigned
        }

        getchar();
        exit (1);
    }

    SetupDiDestroyDeviceInfoList(idTemp); // Limpiamos lista para que no hayan fugas de memoria si ha funcionado.

} // Esta funcion lista los puertos disponibles usando setupAPI.

int PedirPuertoSeriei () {

    int opcionPuerto;
    int opcionTest = leer_entero("\nIntroduzca el COM deseado: ", &opcionPuerto);

    while (opcionTest != 0) {
        opcionTest = leer_entero("\nIntroduzca el COM deseado: ", &opcionPuerto);
    }

    return opcionPuerto;
} // esa i al final significa que es un int.

int PedirBaudiosi () {

    int opcionBaudios;
    int opcionTest = leer_entero("\nIntroduzca los baudios deseados: ", &opcionBaudios);

    while (opcionTest != 0) {
        opcionTest = leer_entero("\nIntroduzca los baudios deseados: ", &opcionBaudios);
    }

    return opcionBaudios;
} // // esa i al final significa que es un int.

int main() {

    // LISTADO Y ELECCIÓN DE PUERTOS SERIE //

    ListarPuertosSerie(); // Listamos. En la función se usa su propio SetConsoleOutput.

    SetConsoleOutputCP(65001); // Pasamos a UTF-8 despues de usar la funcion con ANSI.

    int UserPort = PedirPuertoSeriei();

    printf(RESET"\nHa escogido el puerto "BLUE"(COM%d).\n"RESET, UserPort);

    // ELECCIÓN DE BAUDIOS //

    printf("\nEscoja la opción de tasa de baudios para la comunicación.\n\n");

    printf("1 -> 1200\n2 -> 2400\n3 -> 4800\n4 -> 9600\n5 -> 19200\n6 -> 38400\n7 -> 57600\n8 -> 115200\n");

    int UserBaudsOpcion = PedirBaudiosi();
    int UserBaudsTasa = 0;

    if (UserBaudsOpcion == 1) {
        UserBaudsTasa = 1200;
    } else if (UserBaudsOpcion == 2) {
        UserBaudsTasa = 2400;
    } else if (UserBaudsOpcion == 3) {
        UserBaudsTasa = 4800;
    } else if (UserBaudsOpcion == 4) {
        UserBaudsTasa = 9600;
    } else if (UserBaudsOpcion == 5) {
        UserBaudsTasa = 19200;
    } else if (UserBaudsOpcion == 6) {
        UserBaudsTasa = 38400;
    } else if (UserBaudsOpcion == 7) {
        UserBaudsTasa = 57600;
    } else if (UserBaudsOpcion == 8) {
        UserBaudsTasa = 115200;
    } else {
        // Control de errores por si introducen un número fuera del rango 1-8
        printf(RED"\nOpción inválida."RESET" Asignando 9600 baudios por defecto.\n");
        UserBaudsTasa = 9600;
    }

    printf("\nHa escogido la tasa de" BLUE" %d baudios.\n\n"RESET, UserBaudsTasa);

    // MENSAJE, ENVIO //

    char mensaje[100] = "Miau!\n";
    uint8_t buff[100]={0}; // Buffer lleno de ceros.
    HANDLE port  = openSerial("COM9",9600); // Abrimos conexión.

    read_port(port,buff,sizeof(buff));
    // Intenta leer si el STM32 había enviado algo viejo mientras se conectaba.
    // Como el buffer inicial buff se sobrescribe, esto básicamente limpia la línea.

    write_port(port,mensaje,strlen(mensaje)); // Envía texto.

    FlushFileBuffers(port); // Aseguramos que mande el mensaje entero.

    // DISPLAY EN CONSOLA //
    read_port(port,buff,sizeof(buff)); // Escucha respuesta de STM32. Saca el mensaje de la bandeja de COM9.
    printf(buff); // Guardamos respuesta en buffer, y lo imprimimos en la consola.
    CloseHandle(port); // Cerramos el puerto.

    getchar(); // pulsar enter para salir de cmd.
}

