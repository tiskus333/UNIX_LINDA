cmake_minimum_required(VERSION 3.1)

project(evolving-rectangles./)

add_compile_options(-Wall -O2 -std=c++17)

file(GLOB TARGET_SRC "./src/*.cpp" )
##set build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

include_directories(
${CMAKE_HOME_DIRECTORY}/include
${SFML_INCLUDE_DIR}
)

add_executable(LINDA ${TARGET_SRC})