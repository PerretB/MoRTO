
include_directories(${PROJECT_SOURCE_DIR}/include)

find_package (Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "boost  not found")
endif()


add_library(MoRTO SHARED morto.cpp)
install (TARGETS MoRTO DESTINATION lib)
install (FILES ../include/morto.h DESTINATION include)

