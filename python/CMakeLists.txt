option (PYTHON_BINDING
        "Generate python bindings." ON)

if (PYTHON_BINDING)

    if(WIN32)
        set(morto_precompiled_library _mortof.dll)
    else()
        set(morto_precompiled_library _mortof.so)
    endif(WIN32)

    configure_file(setup.py.in ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
    configure_file(MANIFEST.in ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST.in)
    configure_file(README.rst ${CMAKE_CURRENT_BINARY_DIR}/README.rst COPYONLY)
    add_subdirectory(morto)


endif (PYTHON_BINDING)