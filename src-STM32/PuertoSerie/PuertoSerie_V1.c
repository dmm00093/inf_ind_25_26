/* Programa: PuertoSerie_v1
 * Editado por: dmm00093 (original dado por profesor)
 * Funcion:
 *      1. Ofrece un listado de puertos serie disponibles usando QueryDosDevice.
 *      2. Permite habilitar al usuario cualquier puerto serie deseado.
*/

#include <stdio.h>
#include <windows.h>
#include <stdint.h>
#include <setupapi.h>

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

int main() {

    // Listado puertos serie //

    // Funcion: usamos sprintf para almacenar el nombre de cada COM dentro de un string.
    // Iteramos de 0 a 255 ya que ese es el limite de puertos.
    // Luego usamos QueryDosDeviceA que busca ese COM, dentro de un buffer, dentro de un maximo de caracteres.
    // QueryDosDeviceA devolverá un 0 si no encuentra el puerto. Si lo encuentra, hace un return del puerto (string).
    // Ese return se almacena en la variable de buffer (segundo argumento).

    for (int i = 0 ; i <= 255 ; i++) {

        char bufferListado [5000];
        char puerto[20];
        sprintf(puerto, "COM%d", i);

        if ((QueryDosDeviceA(puerto, bufferListado, 5000) == 0)) {
            // printf("Error mostrando puertos.\n");
        }
        else {
            printf("COM%d -> %s\n -> %s", i, bufferListado);
        }
    }

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

