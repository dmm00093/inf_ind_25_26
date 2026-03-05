cmake_minimum_required(VERSION 4.1)
project(inf_ind_25_26 C)

set(CMAKE_C_STANDARD 17)

# Carpeta Headers Librerias
include_directories(lib)

add_executable(lectura_lib lib/lectura.c)

add_executable(ejercicio1 tema3-punt-arrays-funcs/ejercicios-entrega-t3/ejercicio1.c)
add_executable(ejercicio2 tema3-punt-arrays-funcs/ejercicios-entrega-t3/ejercicio2.c)
add_executable(ejercicio3 tema3-punt-arrays-funcs/ejercicios-entrega-t3/ejercicio3.c)
add_executable(ejercicio3bonus tema3-punt-arrays-funcs/ejercicios-entrega-t3/ejercicio3_bonus.c)


add_executable(array1 tema3-punt-arrays-funcs/ejemplos-arrays/array1_basico.c)
add_executable(array2 tema3-punt-arrays-funcs/ejemplos-arrays/array2_leer.c)
add_executable(array3 tema3-punt-arrays-funcs/ejemplos-arrays/array3_inverso.c)
add_executable(array4 tema3-punt-arrays-funcs/ejemplos-arrays/array4_suma_elems.c)

add_executable(ejercicio_t5_A tema5-menus-structs/practica-tema5/ejercicio_t5_a.c)
# Programa + Libreria
add_executable(ejercicio_t5_B tema5-menus-structs/practica-tema5/ejercicio_t5_b.c
               lib/lectura.c
              )
