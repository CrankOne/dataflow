cmake_minimum_required( VERSION 3.1 )
project( Dataflow )

set( Dataflow_VERSION 0.0.1 )

option( BUILD_DOC "Controls generation of documentation using Doxygen" ON )
option( BUILD_TESTS "Controls build of unit tests (Google testing needed)" ON )

find_package( GTest QUIET )

file(GLOB_RECURSE Dataflow_SOURCES src/*.c*)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
      "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel. By default Debug is assumed."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if( ${CMAKE_BUILD_TYPE} STREQUAL "Debug" )
    set( BUILDCONF_SUFFIX dbg )
elseif( ${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel" )
    set( BUILDCONF_SUFFIX minsize )
endif()

if( BUILDCONF_SUFFIX STREQUAL "" )
    set( Dataflow_LIBRARY dataflowf-${BUILDCONF_SUFFIX} )
else()
    set( Dataflow_LIBRARY dataflow )
endif()

add_library( ${Dataflow_LIBRARY} SHARED ${Dataflow_SOURCES} )

target_include_directories( ${Dataflow_LIBRARY} PUBLIC include
    $<INSTALL_INTERFACE:include/libdataflow> )

set_target_properties( ${Dataflow_LIBRARY} PROPERTIES
    VERSION ${Dataflow_VERSION}
    SOVERSION ${Dataflow_VERSION} )

#
# Unit testing

if( BUILD_TESTS )
    if( GTEST_FOUND )
        file( GLOB_RECURSE Dataflow_tests_SOURCES tests/*.c* )
        if( BUILDCONF_SUFFIX STREQUAL "" )
            set( Dataflow_UNITTESTS dataflow-tests )
        else()
            set( Dataflow_UNITTESTS dataflow-tests-${BUILDCONF_SUFFIX} )
        endif()
        add_executable( ${Dataflow_UNITTESTS} ${Dataflow_tests_SOURCES} )
        target_include_directories( ${Dataflow_UNITTESTS} PUBLIC include SYSTEM ${GTEST_INCLUDE_DIRS} )
        target_link_libraries( ${Dataflow_UNITTESTS} ${Dataflow_LIBRARY} ${GTEST_BOTH_LIBRARIES} )
    else()
        message( WARNING "gtest was not found -- no unit tests will be built." )
    endif()
endif( BUILD_TESTS )


#
# Documentation

if( BUILD_DOC )
    find_package( Doxygen QUIET )
    # check if Doxygen is installed
    if( DOXYGEN_FOUND )
        set(STL_CPP_TAGFILE ${CMAKE_BINARY_DIR}/cppreference-doxygen-web.tag.xml)
        # Try to fetch the web tag file for STL references.
        if( NOT EXISTS ${STL_CPP_TAGFILE} )
            file( DOWNLOAD http://upload.cppreference.com/mwiki/images/f/f8/cppreference-doxygen-web.tag.xml
                           ${STL_CPP_TAGFILE}
                           TIMEOUT 20
                           EXPECTED_HASH SHA1=1df9e91490837590c5bb1f40c8cbad146370d00c
                           TLS_VERIFY ON )
        endif()
        configure_file( Doxyfile.in Doxyfile @ONLY )
        if( EXISTS ${STL_CPP_TAGFILE} )
            message(STATUS "STL reference links will be included in documentation.")
            # TODO: this below does not work. Figure out what is going on and
            # remove TAGFILES=@STL... crutch from Doxyfile.in.
            #file(APPEND "Doxyfile" "TAGFILES+=\"${STL_CPP_TAGFILE}=http://en.cppreference.com/w/\"")
        endif()
        # Note the option ALL which allows to build the docs together with the
        # rest of project
        add_custom_target( doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    else (DOXYGEN_FOUND)
        message(WARNING "Doxygen is not found, documentation won't be generated.")
    endif (DOXYGEN_FOUND)
endif( BUILD_DOC )
