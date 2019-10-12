get_filename_component(MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(SRC main.cpp)
set(TESTS test_main.cpp)

add_library(${MODULE_NAME} STATIC ${SRC})
add_executable(${MODULE_NAME}_tests ${TESTS})
target_link_libraries(${MODULE_NAME}_tests gtest_main gtest pthread)

add_test(NAME ${MODULE_NAME}_tests COMMAND ${MODULE_NAME}_tests
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
