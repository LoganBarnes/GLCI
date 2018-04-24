###############################################################
# Copyright (c) 2018. All rights reserved.
###############################################################
cmake_minimum_required(VERSION 3.5.1)
project(GLCI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GLCI_OFFSCREEN "Use offscreen OpenGL context creation" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

######################
### EXTERNAL REPOS ###
######################
include(DownloadProject)

# Download and unpack gl3w at configure time
download_project(PROJ gl3w
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/skaslev/gl3w.git
        GIT_TAG master
        UPDATE_DISCONNECTED 1
        QUIET
        )

execute_process(COMMAND ./gl3w_gen.py WORKING_DIRECTORY ${gl3w_SOURCE_DIR})

add_library(gl3w ${gl3w_SOURCE_DIR}/src/gl3w.c)
target_include_directories(gl3w SYSTEM PUBLIC ${gl3w_SOURCE_DIR}/include)

# Download and unpack glfw at configure time
download_project(PROJ glfw
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG master
        UPDATE_DISCONNECTED 1
        QUIET
        )

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# compile glfw with current project
add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})

add_library(thirdparty INTERFACE)
target_link_libraries(thirdparty INTERFACE glfw gl3w)

#############################
### Project Configuration ###
#############################
if (NOT MSVC)
    set(INTENSE_FLAGS -Wall -Wextra -Winit-self -Winit-self
            -Wold-style-cast -Woverloaded-virtual -Werror
            -Wmissing-declarations -pedantic -Wuninitialized)
else ()
    set(INTENSE_FLAGS -WX)
endif ()

if (MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS) # silly microsoft
endif ()

############################
### Visual Music Project ###
############################

set(GLCI_SOURCE_FILES
        src/glci/GLInstance.cpp
        src/glci/GLInstance.hpp
        src/main.cpp
        )

add_executable(glci ${GLCI_SOURCE_FILES})
target_include_directories(glci PRIVATE src)
target_link_libraries(glci thirdparty)
target_compile_options(glci PRIVATE ${INTENSE_FLAGS})
