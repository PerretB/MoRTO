option (PYTHON_BINDING
        "Generate python bindings." ON)

if (PYTHON_BINDING)
    FIND_PACKAGE(SWIG REQUIRED)
    INCLUDE(${SWIG_USE_FILE})

    include(findNumpy.cmake)
    if(PYTHON_NUMPY_FOUND)

    else ()
        message(FATAL_ERROR "numpy  not found")
    endif(PYTHON_NUMPY_FOUND)

    FIND_PACKAGE(PythonLibs)
    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/python ${PYTHON_NUMPY_INCLUDE_DIR})

    SET(CMAKE_SWIG_FLAGS "")

    SET_SOURCE_FILES_PROPERTIES(morto.i PROPERTIES CPLUSPLUS ON)
    SET_SOURCE_FILES_PROPERTIES(morto.i PROPERTIES SWIG_FLAGS "-includeall")
    SWIG_ADD_MODULE(morto python morto.i morto_python.cpp)
    SWIG_LINK_LIBRARIES(morto ${PYTHON_LIBRARIES}  MoRTO)

    if (DO_TEST)
        configure_file(run_test.py ${CMAKE_CURRENT_BINARY_DIR}/run_test.py COPYONLY)
        find_package(PythonInterp REQUIRED)
        add_test(NAME python_binding
                COMMAND ${PYTHON_EXECUTABLE}
                ${CMAKE_CURRENT_BINARY_DIR}/run_test.py
                )
        set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} _morto PARENT_SCOPE)
    endif (DO_TEST)


endif (PYTHON_BINDING)