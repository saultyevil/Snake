cmake_minimum_required(VERSION 3.10)
project(Snake C)

set(CMAKE_C_STANDARD 99)
add_executable(snake
        src/functions.h src/main.c src/eddington.c src/snake.h
        src/read_pars.c src/setup.c src/time.c src/utility.c src/default_pars.c
        src/grid_set.c src/output.c)

target_link_libraries(snake m)
