cmake_minimum_required(VERSION 2.8)
project(GlueExample)

set (CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic")

# Including Glue
set(GLUE "${CMAKE_SOURCE_DIR}/../"
    CACHE STRING "Glue directory")

include("${GLUE}/Glue.cmake")

# Registering Glue classes
glue_parse(Printer.h)
glue_parse(Constant.h)
glue_is_convertible(float int)

# Building library
set(ALL_SOURCES
    Printer.cpp
    )

add_library(Example ${ALL_SOURCES})
glue_run(Example)
