project(template_cmake_project)
cmake_minimum_required(VERSION 3.0)
set(LIBRARY_OUTPUT_DIRECTORY ./lib)
set(BINARY_OUTPUT_DIRECTORY ./bin)

find_package(GTest REQUIRED)
find_library(gmock REQUIRED)

set(SRC src/main.cpp)
set(TESTS src/test_main.cpp)

add_executable(${PROJECT_NAME} ${SRC})
add_executable(${PROJECT_NAME}_tests ${TESTS})
target_link_libraries(${PROJECT_NAME}_tests gtest_main gtest)

enable_testing()
add_test(${PROJECT_NAME}_tests ${PROJECT_NAME}_tests)
