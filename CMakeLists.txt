cmake_minimum_required( VERSION 3.1 )
project( Dataflow )

set( Dataflow_VERSION 0.0.1 )

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

