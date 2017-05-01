cmake_minimum_required(VERSION 3.0)
project(MoRTO)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

set(SOURCE_FILES
        src/morto.cpp
        )

set(TEST_FILES test/test.cpp)



option (DO_TEST
        "Execute test suit." ON)

if (DO_TEST)
    enable_testing()
endif(DO_TEST)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/test)

add_subdirectory(${PROJECT_SOURCE_DIR}/python)



if (DO_TEST)
    add_custom_target( all_tests ALL
            DEPENDS ${UNIT_TEST_TARGETS}
            )

    add_custom_command(TARGET all_tests
            COMMENT "Run tests"
            POST_BUILD COMMAND ctest ARGS --output-on-failure
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
endif(DO_TEST)