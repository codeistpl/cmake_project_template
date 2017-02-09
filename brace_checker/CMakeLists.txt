get_filename_component(MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB SRC ./*.cpp)
list(LENGTH SRC SRC_SIZE)
file(GLOB TESTS ./*_test.cpp)
list(LENGTH TESTS TESTS_SIZE)
file(GLOB MOCKS ./*_mock.cpp)
list(LENGTH MOCKS MOCKS_SIZE)

list(REMOVE_ITEM SRC ${TESTS})
list(REMOVE_ITEM SRC ${MOCKS})


message("src" ${SRC})
message("tests" ${TESTS})
message("mocks" ${MOCKS})
add_library(${MODULE_NAME} STATIC ${SRC})
add_executable(${MODULE_NAME}_tests ${TESTS})

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

target_link_libraries(${MODULE_NAME}_tests ${MODULE_NAME} GTest::GTest GTest::Main)

add_test(NAME ${MODULE_NAME}_tests COMMAND ${MODULE_NAME}_tests
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
