cmake_minimum_required(VERSION 4.1)
project(inf_ind_25_26 C)

set(CMAKE_C_STANDARD 17)

# Carpeta Headers Librerias
include_directories(lib)

add_executable(lectura_lib lib/lectura.c)
add_executable(productos_lib lib/productos.c)

add_executable(ejercicio1 src-temas/tema3-punt-arrays-funcs/ejercicios-entrega-t3/ejercicio1.c)
add_executable(ejercicio2 src-temas/tema3-punt-arrays-funcs/ejercicios-entrega-t3/ejercicio2.c)
add_executable(ejercicio3 src-temas/tema3-punt-arrays-funcs/ejercicios-entrega-t3/ejercicio3.c)
add_executable(ejercicio3bonus src-temas/tema3-punt-arrays-funcs/ejercicios-entrega-t3/ejercicio3_bonus.c)


add_executable(array1 src-temas/tema3-punt-arrays-funcs/ejemplos-arrays/array1_basico.c)
add_executable(array2 src-temas/tema3-punt-arrays-funcs/ejemplos-arrays/array2_leer.c)
add_executable(array3 src-temas/tema3-punt-arrays-funcs/ejemplos-arrays/array3_inverso.c)
add_executable(array4 src-temas/tema3-punt-arrays-funcs/ejemplos-arrays/array4_suma_elems.c)

add_executable(ejercicio_t5_A src-temas/tema5-menus-structs/practica-tema5/ejercicio_t5_a.c)
# Programa + Libreria
add_executable(ejercicio_t5_B src-temas/tema5-menus-structs/practica-tema5/ejercicio_t5_b.c
               lib/lectura.c
              )
# Programa + Librerias
add_executable(maquina_expendedora src-temas/tema5-menus-structs/ej-maquina-exp/maquina_expend.c
               lib/lectura.c
)

add_executable(maq_exp_ficheros src-maq-exp/maq_exp_fich.c
              lib/lectura.c
)

add_executable(ej1-mem-din src-temas/tema7-memoria/ej1-memoria-dinamica.c
              lib/lectura.c
)
add_executable(ej1-scanf src-temas/tema7-memoria/ej-1_scanf.c)

# STM32
add_executable(PuertoSerie_Comm src-STM32/PuertoSerie/PuertoSerie_Comm.c)
add_executable(PuertoSerie_V1 src-STM32/PuertoSerie/PuertoSerie_V1.c)
add_executable(PuertoSerie_V2 src-STM32/PuertoSerie/PuertoSerie_V2.c
               lib/lectura.c)
target_link_libraries(PuertoSerie_V2 setupapi) # Necesario para que funcione setupAPI.