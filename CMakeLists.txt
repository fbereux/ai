cmake_minimum_required(VERSION 2.6)

#Configuration du projet
project(ai CXX C)

#Configuration du type de compilation
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release."
      FORCE)
endif (NOT CMAKE_BUILD_TYPE)
set(CMAKE_VERBOSE_MAKEFILE OFF CACHE BOOL "Switch ON/OFF for verbose make" FORCE)

#Configuration des options de compilation
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -pg -g")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

#Configuration de l'exécutable
file(
        GLOB_RECURSE
        source_files
        src/*
)
include_directories(
    ${PROJECT_SOURCE_DIR}/src/
)

add_executable(ai ${source_files})

