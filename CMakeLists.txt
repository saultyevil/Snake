cmake_minimum_required(VERSION 3.10)
project(Snake C Fortran)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_Fortran_COMPILER gfortran)
add_executable(snake
        src/functions.h src/main.c src/eddington.c src/snake.h
        src/read_pars.c src/geo_init.c src/time.c src/utility.c src/default_pars.c
        src/grid_init.c src/output.c src/convergence.c src/flib.h
        src/opal.f src/density_init.c)

target_link_libraries(snake m)
