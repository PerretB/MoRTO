option (PYTHON_BINDING
        "Generate python bindings." ON)

option (INSTALL_PYTHON_BINDING
        "Install python bindins with disutil." ON)

if (PYTHON_BINDING)


    if(WIN32)
        set(morto_precompiled_library _morto.dll)
    else()
        set(morto_precompiled_library _morto.so)
    endif(WIN32)

    configure_file(setup.py.in ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
    configure_file(MANIFEST.in ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST.in)

    add_subdirectory(morto)

endif (PYTHON_BINDING)