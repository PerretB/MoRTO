
if (DO_TEST)
    find_package(Boost 1.55 REQUIRED COMPONENTS unit_test_framework)
    include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

    add_executable(test_exe test.cpp)
    ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
    target_link_libraries(test_exe ${Boost_LIBRARIES} MoRTO)
    add_test(NAME Test_cpp COMMAND test_exe)
    set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} test_exe CACHE INTERNAL "" FORCE)
endif (DO_TEST)