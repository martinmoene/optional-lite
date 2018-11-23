# Copyright 2016-2018 by Martin Moene
#
# https://github.com/martinmoene/optional-lite
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 3.5 )

# optional-lite project and version, updated by script/update-version.py:

project( optional_lite LANGUAGES CXX )

set( optional_lite_version "3.1.1" )

# Toplevel or subproject:

if ( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    set( optional_IS_TOPLEVEL_PROJECT TRUE )
else()
    set( optional_IS_TOPLEVEL_PROJECT FALSE )
endif()

# Enable building and performing of tests and building of examples:

option( OPTIONAL_LITE_OPT_BUILD_TESTS    "Build and perform optional-lite tests" ${optional_IS_TOPLEVEL_PROJECT} )
option( OPTIONAL_LITE_OPT_BUILD_EXAMPLES "Build optional-lite examples" OFF )

option( OPTIONAL_LITE_OPT_SELECT_STD     "Select std::optional"    OFF )
option( OPTIONAL_LITE_OPT_SELECT_NONSTD  "Select nonstd::optional" OFF )

include( GNUInstallDirs )

set( package_name "optional-lite" )
set( include_source_dir "${PROJECT_SOURCE_DIR}/include" )

# Interface library:

add_library(
    ${package_name} INTERFACE )

target_include_directories(
    ${package_name} INTERFACE "$<BUILD_INTERFACE:${include_source_dir}>" )

# Installation:

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

# If requested, build and perform tests, build examples:

enable_testing()

if ( OPTIONAL_LITE_OPT_BUILD_TESTS )
    add_subdirectory( test )
endif()

if ( OPTIONAL_LITE_OPT_BUILD_EXAMPLES )
    add_subdirectory( example )
endif()

# end of file
