project(template_cmake_project)
cmake_minimum_required(VERSION 3.0)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include(googletest.cmake)

set(SRC src/main.cpp)
set(TESTS src/test_main.cpp)

add_library(static_${PROJECT_NAME} STATIC ${SRC})
add_executable(${PROJECT_NAME} ${SRC})
add_executable(${PROJECT_NAME}_tests ${TESTS})

target_link_libraries(${PROJECT_NAME}_tests static_${PROJECT_NAME} gtest_main gtest pthread)

enable_testing()
add_test(unit_tests ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}_tests)
