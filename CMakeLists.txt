cmake_minimum_required(VERSION 3.0)

project(PROJ1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_EXTENSIONS ON)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(FLTK REQUIRED)

add_executable(proj1)

target_include_directories(proj1 PUBLIC ./)
target_include_directories(proj1 PUBLIC ./fltk-1.3.3/)

target_link_libraries(proj1 PRIVATE ${OPENGL_LIBRARIES})

include_directories(${FLTK_INCLUDE_DIR})
target_link_libraries(proj1 PRIVATE ${FLTK_LIBRARIES})

# add_definitions(-DPROJ_DEBUG)
add_definitions(-DENABLE_DEBUGGER)
add_definitions(-DMINIMIZE_WINDOW_EXIT)
add_definitions(-D_MSC_VER=1300)
add_definitions(-DGL_SILENCE_DEPRECATION)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-nonportable-include-path")

file(GLOB SRC_FILES    
    "*.cpp"
    "*.cxx"
    "*.hpp"
)

target_sources(proj1 PRIVATE 
    ${SRC_FILES}
)