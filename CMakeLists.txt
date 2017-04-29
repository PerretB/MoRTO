cmake_minimum_required(VERSION 3.7)
project(MoRTO)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

set(SOURCE_FILES
        src/morto.cpp
        src/mortocl.cpp)

set(TEST_FILES test/test.cpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/test)
