cmake_minimum_required(VERSION 3.13)
project(MST
        LANGUAGES CXX
        VERSION 1.0.0
        DESCRIPTION "CSE 6140 Assignment 2: MST implementation")

# -------------------------------------------------------------
# Default build type

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# -------------------------------------------------------------
# C/CXX Standard and Flags

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags
if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4 /WX /DDEBUG")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
endif ()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -------------------------------------------------------------
# Other flags

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


# Compile the main executable
add_executable(mst src/main.cpp src/graph.cpp)
target_include_directories(mst PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src")

# -------------------------------------------------------------
# Testing
enable_testing()
add_subdirectory(test)

# -------------------------------------------------------------
# Docs
add_subdirectory(docs)
