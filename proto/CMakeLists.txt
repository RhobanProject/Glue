cmake_minimum_required(VERSION 2.8)
project(Proto)

set (CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic")

# Including Glue
set(GLUE "${CMAKE_SOURCE_DIR}/../"
    CACHE STRING "Glue directory")

include("${GLUE}/Glue.cmake")

# Building library
set(ALL_SOURCES
    main.cpp 
    Printer.cpp
    )

glue_add(convert.cpp)
glue_add(generated/GlueConstant.cpp)
glue_add(generated/GluePrinter.cpp)
glue_add(generated/glue.cpp)

glue_run()
add_executable(proto ${ALL_SOURCES})
target_link_libraries(proto glue)
