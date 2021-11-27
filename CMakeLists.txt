cmake_minimum_required(VERSION 3.18)

project(sps LANGUAGES CXX)

find_package(Boost COMPONENTS unit_test_framework serialization REQUIRED)
find_package (Python COMPONENTS Interpreter Development REQUIRED)
add_subdirectory(extern/pybind11)

add_library(
    sps
    INTERFACE
    src/probtree.h
    src/node.h
    )
target_link_libraries(sps INTERFACE Boost::serialization ${PYTHON_LIBRARIES})
target_include_directories(sps INTERFACE ${PYTHON_INCLUDE_DIRS})
target_include_directories(sps INTERFACE extern/pybind11/include)

set_target_properties(sps
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

add_executable(main src/main.cpp)
target_link_libraries(main sps)

pybind11_add_module(
    sps_c

    src/probtree.h
    src/node.h
    src/main.cpp
    )
set_target_properties(sps_c PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(sps_c INTERFACE sps)
target_compile_definitions(sps_c PRIVATE)

add_executable(test_sps test/test_sps.cpp)
target_link_libraries(test_sps Boost::unit_test_framework sps)

