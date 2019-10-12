include(ExternalProject)
ExternalProject_Add(googletest
  URL https://github.com/google/googletest/archive/release-1.8.0.zip
  URL_HASH SHA1=667f873ab7a4d246062565fad32fb6d8e203ee73
  INSTALL_COMMAND ""
)
 
ExternalProject_Get_Property(googletest binary_dir)
ExternalProject_Get_Property(googletest source_dir)

add_library(gtest      UNKNOWN IMPORTED)
add_library(gtest_main UNKNOWN IMPORTED)
add_library(gmock      UNKNOWN IMPORTED)
add_library(gmock_main UNKNOWN IMPORTED)

set_target_properties(gtest PROPERTIES
  IMPORTED_LOCATION ${binary_dir}/googlemock/gtest/libgtest.a
)

set_target_properties(gtest_main PROPERTIES
  IMPORTED_LOCATION ${binary_dir}/googlemock/gtest/libgtest_main.a
)

set_target_properties(gmock PROPERTIES
  IMPORTED_LOCATION ${binary_dir}/googlemock/libgmock.a
)

set_target_properties(gmock_main PROPERTIES
  IMPORTED_LOCATION ${binary_dir}/googlemock/libgmock_main.a
)

include_directories(${source_dir}/googletest/include ${source_dir}/googlemock/include)

add_dependencies(gtest      googletest)
add_dependencies(gtest_main googletest)
add_dependencies(gmock      googletest)
add_dependencies(gmock_main googletest)
