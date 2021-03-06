cmake_minimum_required(VERSION 2.8)
project(GlueExample)

set (CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic")

# Including Glue
set(GLUE "${CMAKE_SOURCE_DIR}/../"
    CACHE STRING "Glue directory")

include("${GLUE}/Glue.cmake")

# Registering Glue classes
glue_parse(Gain.h)
glue_parse(Add.h)
glue_parse(Printer.h)
glue_parse(Constant.h)
glue_is_convertible(float int)
glue_is_convertible(int "std::string")
glue_is_convertible(float "std::string")

# Building library
set(ALL_SOURCES
    convert.cpp
    main.cpp
    Constant.cpp
    Printer.cpp
    Gain.cpp
    Add.cpp
    )

glue_run()
add_executable(Example ${ALL_SOURCES})
target_link_libraries(Example glue)
