cmake_minimum_required(VERSION 3.10)
project(Snake C Fortran)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_Fortran_COMPILER gfortran)
add_executable(snake
        src/snake_functions.h src/main.c src/eddington.c src/snake.h
        src/read_pars.c src/geo_init.c src/time.c src/utility.c src/snake_init.c
        src/grid_init.c src/output.c src/convergence.c src/flib/flib.h
        src/flib/opal.f src/density_init.c src/update_opac.c src/2d_interp.h src/2d_interp.c)

# add_definitions(-DDEBUG)
find_package(GSL REQUIRED)
target_link_libraries(snake m GSL::gsl GSL::gslcblas)
