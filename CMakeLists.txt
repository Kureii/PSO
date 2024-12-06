cmake_minimum_required(VERSION 3.30)
project(PSO)

set(CMAKE_CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

set(MATPLOTLIBCPP_DIR lib/matplotlib-cpp)
include_directories(${MATPLOTLIBCPP_DIR})

add_executable(${PROJECT_NAME}
        main.cpp
        pso_structs.h
        pso.h
        pso.cpp
        test_functions.cpp
        test_functions.h
        helpfull_structs.h
        helpfull_functions.cpp
        helpfull_functions.h
        save_results.cpp
        save_results.h
)

find_package(Python3 COMPONENTS Development NumPy REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
        python_module
        ${Python3_INCLUDE_DIRS}
        ${Python3_NumPy_INCLUDE_DIRS}
)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
else ()
    set( CMAKE_CXX_FLAGS  "-O3")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${Python3_LIBRARIES})
