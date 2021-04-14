cmake_minimum_required(VERSION 3.1)

project(linda)
set(CMAKE_CXX_COMPILER  "/usr/bin/g++-7")
set(CMAKE_CXX_FLAGS "-std=c++1z")
add_compile_options(-Wall -O2 -std=c++17)

file(GLOB TARGET_SRC "./src/*.cpp")
##set build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

include_directories(
${CMAKE_HOME_DIRECTORY}/include
)

add_executable(LINDA ${TARGET_SRC})

#Add tests 
list(FILTER TARGET_SRC EXCLUDE REGEX /*/main.cpp) 
find_package(Boost COMPONENTS unit_test_framework REQUIRED) 
add_executable(basic_test ${CMAKE_HOME_DIRECTORY}/tests/tests_main.cpp ${TARGET_SRC}) 
target_link_libraries(basic_test ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(AutoTest basic_test) 
enable_testing()

