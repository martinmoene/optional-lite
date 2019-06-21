# Copyright 2017-2018 by Martin Moene
#
# https://github.com/martinmoene/optional-lite
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

if( NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION )
    cmake_minimum_required( VERSION 3.5 FATAL_ERROR )
endif()

project( example LANGUAGES CXX )

set( unit_name "optional" )
set( PACKAGE   ${unit_name}-lite )
set( PROGRAM   ${unit_name}-lite )

message( STATUS "Subproject '${PROJECT_NAME}', examples '${PROGRAM}-*'")

set( OPTIONS "" )

if( MSVC )
    message( STATUS "Matched: MSVC")

    set( BASE_OPTIONS -W3 )
    set( EXCEPTIONS_OPTIONS ${BASE_OPTIONS} -EHsc )
    set( NO_EXCEPTIONS_OPTIONS ${BASE_OPTIONS} )

elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang" )
    message( STATUS "CompilerId: '${CMAKE_CXX_COMPILER_ID}'")

    set( BASE_OPTIONS -Wall -Wextra -Wconversion -Wsign-conversion -Wno-missing-braces -fno-elide-constructors )
    set( EXCEPTIONS_OPTIONS ${BASE_OPTIONS} )
    set( NO_EXCEPTIONS_OPTIONS -fno-exceptions )

elseif( CMAKE_CXX_COMPILER_ID MATCHES "Intel" )
    # as is
    message( STATUS "Matched: Intel")
else()
    # as is
    message( STATUS "Matched: nothing")
endif()

function( make_target name no_exceptions )
    add_executable             ( ${PROGRAM}-${name} ${name}.cpp )
    target_link_libraries      ( ${PROGRAM}-${name} PRIVATE ${PACKAGE} )
    if ( no_exceptions )
        target_compile_options ( ${PROGRAM}-${name} PRIVATE ${NO_EXCEPTIONS_OPTIONS} )
    else()
        target_compile_options ( ${PROGRAM}-${name} PRIVATE ${EXCEPTIONS_OPTIONS} )
    endif()

endfunction()

make_target( 01-to_int FALSE )
make_target( 02-nodefltctor FALSE )
# 04-any-optional-variant also requires variant-lite.
make_target( 05-no-exceptions TRUE )

# end of file
