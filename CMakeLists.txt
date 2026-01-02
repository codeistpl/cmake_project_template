project(template_cmake_project)
cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,cppcoreguidelines-*,performance-*,clang-diagnostic-*,clang-analyzer-*,modernize-*')
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Restrictive compilation flags
add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  -Werror
  -Wshadow
  -Wconversion
  -Wunused
  -Wunused-parameter
)

# Enable sanitizers
add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/gcovr.cmake)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

enable_testing()
add_subdirectory(brace_checker)

add_custom_target(format
  COMMAND find ${CMAKE_CURRENT_SOURCE_DIR} -regex '.*\\.\\(cpp\\|h\\|hpp\\)' -exec clang-format -style=file -i {} \;
  COMMENT "Running clang-format on C++ source files"
)

add_custom_target(lint
    COMMAND clang-tidy -p=${CMAKE_BINARY_DIR} --header-filter=.* ${CMAKE_CURRENT_SOURCE_DIR}/brace_checker/*.cpp
    COMMENT "Running clang-tidy linter"
    DEPENDS brace_checker
)
