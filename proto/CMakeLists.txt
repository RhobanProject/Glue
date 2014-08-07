cmake_minimum_required(VERSION 2.8)
project(Proto)

set (CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic")

# Including Glue
set(GLUE "${CMAKE_SOURCE_DIR}/../"
    CACHE STRING "Glue directory")

include("${GLUE}/Glue.cmake")

glue_add(convert.cpp)
glue_add(deserialize.cpp)

# Building library
set(ALL_SOURCES
    generated/GlueConstant.cpp 
    generated/GluePrinter.cpp 
    generated/glue.cpp 

    main.cpp 
    Printer.cpp
    )

add_library(Proto ${ALL_SOURCES})
glue_run(Proto)
