# The ${GLUE} variable should point to the Glue source directory
if (NOT DEFINED GLUE)
    message(FATAL_ERROR "You should set the \$GLUE variable")
endif()

# JSON Support
add_subdirectory("${GLUE}/json" json)
set(JSON_HEADERS "${GLUE}/json/include" 
    CACHE STRING "Json headers")
include_directories("${JSON_HEADERS}")

# Mongoose web support
add_subdirectory("${GLUE}/mongoose/" mongoose)

# Output directory is current build/glue
set(GLUE_OUTPUT_DIR "${PROJECT_BINARY_DIR}/glue/")
include_directories("${GLUE}/include/" "${GLUE_OUTPUT_DIR}")

# Glue generator
set(GLUE_GENERATOR "${GLUE}/generator/generator.py")

# Append files to be parsed
set(GLUE_FILES "" CACHE STRING "Glue files")
set(GLUE_ADDITIONAL "" CACHE STRING "Glue additional files")
set(GLUE_GENERATED_FILES "" CACHE STRING "Glue generating files")

macro(glue_parse_absolute file)
    get_filename_component(component ${file} NAME_WE)
    set(component "${GLUE_OUTPUT_DIR}/Glue${component}.cpp")
    set(GLUE_GENERATED_FILES "${GLUE_GENERATED_FILES}" "${component}")
    set(GLUE_FILES "${GLUE_FILES}" "${file}")
endmacro()

macro(glue_parse file)
    glue_parse_absolute("${CMAKE_SOURCE_DIR}/${file}")
endmacro()

macro(glue_add file)
    set(GLUE_ADDITIONAL ${GLUE_ADDITIONAL} ${file})
endmacro()

# Run the glue dependences
macro(glue_run)
    set(GLUE_GENERATED_FILES
        ${GLUE_GENERATED_FILES}
        "${GLUE_OUTPUT_DIR}/glue.cpp"
    )

    add_custom_command(
        OUTPUT ${GLUE_GENERATED_FILES}
        COMMAND "${GLUE_GENERATOR}" "${GLUE_OUTPUT_DIR}" ${GLUE_FILES}
        DEPENDS ${GLUE_FILES}
    )

    set (GLUE_SOURCES
        ${GLUE_GENERATED_FILES}
        ${GLUE_ADDITIONAL}
        "${GLUE}/Scene.cpp"
        "${GLUE}/deserialize.cpp"
    )

    add_library(glue ${GLUE_SOURCES})
    target_link_libraries(glue json _mongoose)
endmacro()
