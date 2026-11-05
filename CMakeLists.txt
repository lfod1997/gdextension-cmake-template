cmake_minimum_required(VERSION 3.25)

# The project to copy the built artifacts into
set(GODOT_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/project" CACHE PATH "Directory of a Godot project. For addons, this should be its demo project; for in-house modules, this should be your Godot project.")

# The minimal Godot version that you plan to support; if changed, modify the manifest accordingly
set(GODOTCPP_API_VERSION 4.5)

# Library name, must not be empty
set(LIBNAME "gdext") # TODO: Change this name if needed, see README

# If not empty, this CMake project is considered an addon, artifacts will be copied to a standard path
set(ADDON "") # TODO: Change or remove this name, see README

# Enforce C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Version-controlled Dependencies
include(FetchContent)
FetchContent_Declare(
    godot-cpp
    GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
    GIT_TAG "10.0.0-stable"
    GIT_SHALLOW TRUE
    SYSTEM
)

# Fetch all dependencies
message("Fetching godot-cpp")
FetchContent_MakeAvailable(godot-cpp)

# TODO: `add_subdirectory`, or `FetchContent_MakeAvailable` other packages here, after godot-cpp

# When compiling for emscripten, this toolchain hack needs to be included
# set(CMAKE_PROJECT_<project-name>_INCLUDE ${godot-cpp_SOURCE_DIR}/cmake/emsdkHack.cmake)

# Include godot-cpp's main CMake module
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${godot-cpp_SOURCE_DIR}/cmake")
include(GodotCPPModule)

# Retrieve Godot-related properties from the godot::cpp target
get_target_property(GODOTCPP_SUFFIX godot::cpp GODOTCPP_SUFFIX)
get_target_property(GODOTCPP_PLATFORM godot::cpp GODOTCPP_PLATFORM)

# Project declaration
if (ADDON)
    # Native addon facing the public
    project(${ADDON}
        VERSION 0.1
        DESCRIPTION "GDExtension with CMake."
        HOMEPAGE_URL "https://example.com"
        LANGUAGES CXX
    )
else ()
    # In-house native module
    project("${LIBNAME}_cmake" LANGUAGES CXX)
endif ()

# Resolve find_package dependencies
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# TODO: If you have dependencies that require `find_package`, put them here

# Main target
add_library(${LIBNAME} SHARED)
target_sources(${LIBNAME}
    PRIVATE
    src/register_types.cpp
    src/register_types.h
)
target_link_libraries(${LIBNAME}
    PRIVATE godot-cpp
)
set_target_properties(${LIBNAME}
    PROPERTIES
    # Godot does not use a "lib" prefix, they use a standardized suffix instead
    PREFIX ""
    OUTPUT_NAME "${LIBNAME}${GODOTCPP_SUFFIX}"
)

# Doc generation
file(GLOB_RECURSE DOC_XML LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/doc_classes/*.xml")
if (DOC_XML)
    if (GODOTCPP_TARGET MATCHES "editor|template_debug")
        target_doc_sources(${LIBNAME} "${DOC_XML}")
    endif ()
endif ()

# Godot relies on a manifest (a .gdextension file) to load the correct binary
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}.gdextension")
    message(FATAL_ERROR "Missing manifest '${LIBNAME}.gdextension', make sure to author (or rename and edit) this file.")
endif ()

# Post build: copy artifacts into configured Godot project
block()
    if (ADDON)
        set(_install_prefix "${GODOT_PROJECT_DIR}/addons/${ADDON}")
    else ()
        set(_install_prefix "${GODOT_PROJECT_DIR}")
    endif ()
    add_custom_command(
        TARGET ${LIBNAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}.gdextension" "${_install_prefix}"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${LIBNAME}>" "${_install_prefix}/bin/${GODOTCPP_PLATFORM}/$<TARGET_FILE_NAME:${LIBNAME}>"
    )
endblock()
