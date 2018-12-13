cmake_minimum_required(VERSION 3.10)
project(Snake C Fortran)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_Fortran_COMPILER gfortran)
add_executable(snake
        src/snake_functions.h src/main.c src/eddington/eddington.c src/snake.h
        src/util/read_pars.c src/init/geo_init.c src/util/time.c src/util/utility.c src/util/default_pars.c
        src/init/grid_init.c src/util/output.c src/util/convergence.c src/flib/flib.h
        src/flib/opal.f src/init/density_init.c src/opac/opac.c src/opac/opac.h src/opac/interp.c src/error.h)

add_definitions(-DDEBUG)
find_package(GSL REQUIRED)
target_link_libraries(snake m GSL::gsl GSL::gslcblas)
