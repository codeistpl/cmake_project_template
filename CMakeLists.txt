project(template_cmake_project)
cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,cppcoreguidelines-*,performance-*,clang-diagnostic-*,clang-analyzer-*,modernize-*')
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/gcovr.cmake)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

enable_testing()
add_subdirectory(brace_checker)
