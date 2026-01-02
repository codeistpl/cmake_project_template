cmake_minimum_required(VERSION 3.27)
project(template_cmake_project)
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

# Function to collect and format source/header files from submodules
function(index_sources_from_submodule output_sources output_headers)
  file(GLOB_RECURSE sources GLOB_SOURCE_LIST */*.cpp */*.c)
  file(GLOB_RECURSE headers GLOB_HEADER_LIST */*.h */*.hpp)
  
  # Exclude .build directory
  list(FILTER sources EXCLUDE REGEX "\\.build/")
  list(FILTER headers EXCLUDE REGEX "\\.build/")
  
  # Convert semicolon-separated lists to space-separated strings
  string(REPLACE ";" "\t" sources_str "${sources}")
  string(REPLACE ";" "\t" headers_str "${headers}")
  
  # Return values
  set(${output_sources} "${sources_str}" PARENT_SCOPE)
  set(${output_headers} "${headers_str}" PARENT_SCOPE)
endfunction()

# Enable testing and add subdirectory 
enable_testing()
add_subdirectory(brace_checker)
# Add more subdirectories as needed ...

# Index source and header files from submodules
index_sources_from_submodule(_sources_str _headers_str)

message(STATUS "Source files: ${_sources_str}")
message(STATUS "Header files: ${_headers_str}")

add_custom_target(format
  COMMAND clang-format -style=file -i ${_sources_str} ${_headers_str}
  COMMENT "Running clang-format on C++ source files ${_sources_str} and header files ${_headers_str}"
)

add_custom_target(lint
    COMMAND clang-tidy -p=${CMAKE_BINARY_DIR} --header-filter=.* ${_sources_str}
    COMMENT "Running clang-tidy linter"
)
