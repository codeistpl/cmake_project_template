get_filename_component(MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB SRC ./*.cpp)
file(GLOB TESTS ./*_test.cpp)
file(GLOB MOCKS ./*_mock.cpp)

list(REMOVE_ITEM SRC ${TESTS})
list(REMOVE_ITEM SRC ${MOCKS})

message("Module: " ${MODULE_NAME})
message("src:\t " ${SRC})
message("tests:\t " ${TESTS})
message("mocks:\t " ${MOCKS})

add_library(${MODULE_NAME} STATIC ${SRC})
add_executable(${MODULE_NAME}_tests ${TESTS})

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

target_link_libraries(${MODULE_NAME}_tests ${MODULE_NAME} GTest::GTest GTest::Main)

add_test(NAME ${MODULE_NAME}_tests COMMAND ${MODULE_NAME}_tests
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
