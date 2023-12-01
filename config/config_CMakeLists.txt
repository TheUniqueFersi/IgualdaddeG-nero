cmake_minimum_required(VERSION 3.21)
project(IgualdadDeGenero C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../)

include_directories("C:/allegro/include")
link_directories("C:/allegro/lib")

add_executable(IgualdadDeGenero src/main.c)
target_link_libraries(IgualdadDeGenero PUBLIC allegro_monolith)