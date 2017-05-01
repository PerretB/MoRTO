
include_directories(${PROJECT_SOURCE_DIR}/include)

option (GEN_COMMANDLINE
        "Generate command line tool." ON)


find_package (Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "boost  not found")
endif()



add_library(MoRTO SHARED morto.cpp)
install (TARGETS MoRTO DESTINATION lib)
install (FILES ../include/morto.h DESTINATION include)

if (GEN_COMMANDLINE)
    add_executable(MoRTOCL mortocl.cpp)
    target_link_libraries (MoRTOCL MoRTO)
    install (TARGETS MoRTOCL DESTINATION bin)
endif (GEN_COMMANDLINE)