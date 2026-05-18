// https://gist.github.com/bayasdev/b792df905b91502867331dee3399c043#file-pausar_consola-c

//Pausa la consola pero funciona en cualquier plataforma
//Probado en Windows, Linux y https://www.onlinegdb.com/
//prototipo
void pausarConsola()
//ejemplo del main
int main(){
    printf("Hola mundo!\n");
    pausarConsola();
    return 0;
}
//la funcion como tal
void pausarConsola(){
    while (getchar() != '\n');//limpia buffer de entrada
    printf("Presione ENTER para continuar....");//mensaje al usuario
    getchar();//pausa la consola hasta presionar enter
}

//disfruten!