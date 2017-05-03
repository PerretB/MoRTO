
option (STATIC_PYTHON
        "Static link of library in Python binding." ON)

option (PIP_INSTALL
        "Install python package with pip." ON)

find_package(PythonInterp REQUIRED)
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

include(findNumpy.cmake)
if(PYTHON_NUMPY_FOUND)

else ()
    message(FATAL_ERROR "numpy  not found")
endif(PYTHON_NUMPY_FOUND)

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/python/morto ${PYTHON_NUMPY_INCLUDE_DIR})

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(morto.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(morto.i PROPERTIES SWIG_FLAGS "-includeall")

if(STATIC_PYTHON)
    SWIG_ADD_MODULE(morto python morto.i morto_python.cpp)
    SWIG_LINK_LIBRARIES(morto ${PYTHON_LIBRARIES}  MoRTO)
else()
    SWIG_ADD_MODULE(morto python morto.i morto_python.cpp ${PROJECT_SOURCE_DIR}/src/morto.cpp)
    SWIG_LINK_LIBRARIES(morto ${PYTHON_LIBRARIES})
endif(STATIC_PYTHON)


configure_file(__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)

SET(DIST_FILE morto-${MORTO_VERSION_MAJOR}.${MORTO_VERSION_MINOR}.tar.gz)

add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/${DIST_FILE}
        DEPENDS _morto
        COMMAND ${PYTHON_EXECUTABLE} setup.py sdist
        COMMAND mv ${CMAKE_BINARY_DIR}/python/dist/${DIST_FILE} ${CMAKE_BINARY_DIR}/
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/python
        )

add_custom_target(python_dist_file ALL DEPENDS ${CMAKE_BINARY_DIR}/${DIST_FILE})

install(CODE "execute_process(COMMAND pip install ${CMAKE_BINARY_DIR}/${DIST_FILE})")
#python setup.py sdist

if (DO_TEST)
    configure_file(run_test.py ${CMAKE_CURRENT_BINARY_DIR}/run_test.py COPYONLY)

    add_test(NAME python_binding
            COMMAND ${PYTHON_EXECUTABLE}
            ${CMAKE_CURRENT_BINARY_DIR}/run_test.py
            )
    set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} _morto CACHE INTERNAL "" FORCE)
endif (DO_TEST)



