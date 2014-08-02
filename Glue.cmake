# The ${GLUE} variable should point to the Glue source directory
if (NOT DEFINED GLUE)
    message(FATAL_ERROR "You should set the \$GLUE variable")
endif()

# Output directory is current build/glue
set(GLUE_OUTPUT_DIR "${PROJECT_BINARY_DIR}/glue/")
include_directories("${GLUE_OUTPUT_DIR}")

# Glue generator
set(GLUE_GENERATOR "${GLUE}/generator/generator.py")

# Append files to be parsed
set(GLUE_FILES "" CACHE STRING "Glue files")
macro(glue_parse_absolute file)
    set(GLUE_FILES ${GLUE_FILES} ${file})
endmacro()

macro(glue_parse file)
    glue_parse_absolute("${CMAKE_SOURCE_DIR}/${file}")
endmacro()

# Run the glue dependences
macro(glue_run libs)
    set(GENERATED_FILES
        "${GLUE_OUTPUT_DIR}/GlueTypes.cpp"
    )

    add_custom_command(
        OUTPUT ${GENERATED_FILES}
        COMMAND "${GLUE_GENERATOR}" ${GLUE_FILES}
        DEPENDS ${GLUE_FILES}
    )

    set (GLUE_SOURCES
        ${GENERATED_FILES}
    )

    add_executable(glue ${GLUE_SOURCES})
    target_link_libraries(glue ${libs})
endmacro()
