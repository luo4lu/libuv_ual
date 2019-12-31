project(template)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

file(GLOB_RECURSE CPP_SRC_LISTS src/*.cpp)

add_subdirectory(deps/libuv)

include_directories(src)
include_directories(deps/libuv/include)
include_directories(deps/libcoap/include/coap2)

if(CPP_SRC_LISTS)
    # Compile srcs to static library
    add_library(${PROJECT_NAME} STATIC ${CPP_SRC_LISTS})
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/main.cpp)
    # is BIN project
    add_executable(main ${CMAKE_SOURCE_DIR}/main.cpp)
    if(TARGET ${PROJECT_NAME})
        target_link_libraries(main ${PROJECT_NAME})
        target_link_libraries(main uv)
        target_link_libraries(main coap-2-openssl)
    endif()
    set_target_properties(main PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
endif()

aux_source_directory(test TEST_LISTS)

foreach(TTEST ${TEST_LISTS})
    get_filename_component(FILE_NAME ${TTEST} NAME_WE)
    string(SHA256 TEST_TARGET_NAME ${FILE_NAME})
    add_executable(${TEST_TARGET_NAME} ${TTEST})
    if(TARGET ${PROJECT_NAME})
        target_link_libraries(${TEST_TARGET_NAME} ${PROJECT_NAME})
        target_link_libraries(${TEST_TARGET_NAME} uv)
        target_link_libraries(${TEST_TARGET_NAME} coap-2-openssl)
    endif()
    set_target_properties(${TEST_TARGET_NAME} PROPERTIES OUTPUT_NAME ${FILE_NAME})
    set_target_properties(${TEST_TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
endforeach()

