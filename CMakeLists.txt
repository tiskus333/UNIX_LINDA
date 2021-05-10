cmake_minimum_required(VERSION 3.1)
project(linda)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -O2)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy; -checks=-*,-fuchsia*,bugprone*,cert*,clang-analyzer*,cppcoreguidelines*,google*,hicpp*,-hicpp-signed-bitwise,llvm*,misc*,modernize*,-modernize-use-trailing-return-type,performance*,readability*,-*avoid-c-arrays,-*-braces-around-statements,-*array*-decay")
file(GLOB TARGET_SRC "./src/*.cpp")
##set build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

include_directories(
${CMAKE_HOME_DIRECTORY}/include
)

add_executable(LINDA ${TARGET_SRC})

target_link_libraries(LINDA Threads::Threads rt)

#Add tests 
list(FILTER TARGET_SRC EXCLUDE REGEX /*/main.cpp) 
find_package(Boost COMPONENTS unit_test_framework REQUIRED) 
add_executable(basic_test ${CMAKE_HOME_DIRECTORY}/tests/tests_main.cpp ${TARGET_SRC}) 
target_link_libraries(basic_test ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} Threads::Threads rt)
add_test(AutoTest basic_test) 
enable_testing()

