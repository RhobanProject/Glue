# The ${GLUE} variable should point to the Glue source directory
if (NOT DEFINED GLUE)
    message(FATAL_ERROR "You should set the \$GLUE variable")
endif()

# JSON Support
add_subdirectory("${GLUE}/json" json)
set(JSON_HEADERS "${GLUE}/json/include" 
    CACHE STRING "Json headers")
include_directories("${JSON_HEADERS}")

# Output directory is current build/glue
set(GLUE_OUTPUT_DIR "${PROJECT_BINARY_DIR}/glue_generated/")
include_directories("${GLUE}/include/" "${GLUE_OUTPUT_DIR}")

# Glue generator
set(GLUE_GENERATOR "${GLUE}/generator/generator.py")

# Append files to be parsed
set(GLUE_FILES "" CACHE STRING "Glue files")
set(GLUE_ADDITIONAL "" CACHE STRING "Glue additional files")

macro(glue_parse_absolute file)
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
    set(GENERATED_FILES
        "${GLUE_OUTPUT_DIR}/glue.cpp"
    )

    add_custom_command(
        OUTPUT ${GENERATED_FILES}
        COMMAND "${GLUE_GENERATOR}" "${GLUE_OUTPUT_DIR}" ${GLUE_FILES}
        DEPENDS ${GLUE_FILES}
    )

    set (GLUE_SOURCES
        ${GENERATED_FILES}
        ${GLUE_ADDITIONAL}
        "${GLUE}/Scene.cpp"
        "${GLUE}/deserialize.cpp"
    )

    add_library(glue ${GLUE_SOURCES})
    target_link_libraries(glue ${libs} json)
endmacro()
