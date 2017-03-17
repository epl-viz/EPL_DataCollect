# Copyright (c) 2017, EPL-Vizards
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the EPL-Vizards nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL EPL-Vizards BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required( VERSION 3.5 )
project( EPL_DataCollect )

message( STATUS "Using CMake version: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}\n" )
include( ${PROJECT_SOURCE_DIR}/cmake/cmakeScripLoader.cmake )

include( CMakePackageConfigHelpers )
include( ExternalProject )

######################
# Set some variables #
######################

# Set the binary output directories
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/coveralls-cmake/cmake )

set( CMAKE_CXX_STANDARD          14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_C_STANDARD            11 )
set( CMAKE_C_STANDARD_REQUIRED   ON )

option( DISABLE_TESTS        "Disables all tests"                     OFF )
option( ENABLE_CODE_COVERAGE "Enables code coverage tests"            OFF )
option( ENABLE_MOCKING       "Force enables mocking support"          OFF )
option( USE_CLANG_LIBCXX     "Build with libc++ instead of stdlibc++" OFF )

list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" )

# Some useful variables
set( EPL_DC_TEMPLATES_DIR "${PROJECT_SOURCE_DIR}/cmake/templates" )
set( EPL_DC_INCLUDES_DIR  "${PROJECT_SOURCE_DIR}/include" )
set( PLF_COLONY_DIR       "${PROJECT_SOURCE_DIR}/external/plf_colony/" )

set( CM_ENABLE_MOCKING 0 )
set( CM_BUILD_DIR_ROOT "${PROJECT_SOURCE_DIR}" )

if( ENABLE_MOCKING )
  set( CM_ENABLE_MOCKING 1 )
endif( ENABLE_MOCKING )

if( NOT DEFINED CM_DISSECTOR_EPL_ID_PREFIX )
  set( CM_DISSECTOR_EPL_ID_PREFIX "epl-xdd" )
endif( NOT DEFINED CM_DISSECTOR_EPL_ID_PREFIX )

if( NOT DEFINED CM_PLUGIN_PROTO_NAME )
  set( CM_PLUGIN_PROTO_NAME "Ethernet POWERLINK+XDD" )
endif( NOT DEFINED CM_PLUGIN_PROTO_NAME )

################################
# Set up the build environment #
################################

if( ENABLE_CODE_COVERAGE )
  set( COVERAGE_COMPILER_FLAG "--coverage" )
endif( ENABLE_CODE_COVERAGE )

if( USE_CLANG_LIBCXX )
  set( GCC_STDLIB_FLAGS   "-nodefaultlibs -lc++ -lc++abi" )
  set( CLANG_STDLIB_FLAGS "-stdlib=libc++" )
  set( FS_LIB c++experimental )
else( USE_CLANG_LIBCXX )
  set( GCC_STDLIB_FLAGS   "" )
  set( CLANG_STDLIB_FLAGS "" )
   set( FS_LIB stdc++fs )
endif( USE_CLANG_LIBCXX )

run_git()

add_compiler(
  GNU MIN_VERSION "6.2"
  ALL        "-Wall -Wextra -Wfloat-equal -Wshadow -Wno-unknown-pragmas ${GCC_STDLIB_FLAGS}"
  DEBUG      "-O0 -Werror -fno-omit-frame-pointer" ${COVERAGE_COMPILER_FLAG}
  RELEASE    "-O3"

  C_ALL      "-Wall -Wextra -Wfloat-equal -Wshadow -Wno-unknown-pragmas -std=c11"
  C_DEBUG    "-O0 -Werror -fno-omit-frame-pointer" ${COVERAGE_COMPILER_FLAG}
  C_RELEASE  "-O3"

  SANITIZE   "-fsanitize=${SANITIZERS}"
)

add_compiler(
  Clang MIN_VERSION 3.9
  ALL        "-Weverything -ftemplate-backtrace-limit=0 "
             "-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded ${CLANG_STDLIB_FLAGS}"
             "-Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-exit-time-destructors"
             "-Wno-global-constructors -Wno-switch-enum -Wno-reserved-id-macro"
             "-Wno-documentation-unknown-command -Wno-unknown-pragmas"
             "-Wno-disabled-macro-expansion"
  DEBUG      "-Werror -fno-omit-frame-pointer"
  RELEASE    "-O2"

  C_ALL      "-Weverything -ftemplate-backtrace-limit=0  -std=c11"
             "-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-float-equal"
             "-Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-exit-time-destructors"
             "-Wno-global-constructors -Wno-switch-enum -Wno-reserved-id-macro"
             "-Wno-documentation-unknown-command -Wno-unknown-pragmas"
             "-Wno-disabled-macro-expansion"
  C_DEBUG    "-Werror -fno-omit-frame-pointer"
  C_RELEASE  "-O2"

  SANITIZE "-fsanitize=${SANITIZERS}"
)

add_definitions(-DWS_NORETURN=) # For some reason wireshark needs this

######################
# Check Dependencies #
######################

find_package( Git              REQUIRED )
find_package( PythonInterp 3.5 REQUIRED ) # Exec python with PYTHON_EXECUTABLE
find_package( PythonLibs   3.5 REQUIRED )
find_package( TinyXML2     4.0 REQUIRED )
find_package( Threads          REQUIRED )
find_package( Wireshark        REQUIRED )
find_package( PCAP             REQUIRED )
find_package( PkgConfig        REQUIRED )
pkg_search_module(GLIB REQUIRED glib-2.0)


set( CM_WS_PLUGINS_DIR "${Wireshark_INSTALL_DIR}/${Wireshark_PLUGIN_INSTALL_DIR}" )

message( STATUS "Wireshark version:     ${Wireshark_VERSION_STRING}" )
message( STATUS "Wireshark install dir: ${Wireshark_INSTALL_DIR}" )
message( STATUS "Wireshark plugins dir: ${CM_WS_PLUGINS_DIR}" )
link_directories( ${Wireshark_LIB_DIR} )

if( NOT DISABLE_TESTS )
  set( FUNC_RED_PREFIX "${PROJECT_BINARY_DIR}/funcRedirect" )
  set( FUNC_RED_INSTALL_DIR "${FUNC_RED_PREFIX}/install_dir" )
  set( FUNC_RED_LIB_DIR     "${FUNC_RED_INSTALL_DIR}/lib" )

  set( FUNC_RED_INCLUDE_DIRS "${FUNC_RED_INSTALL_DIR}/include/funcRedirect" )

  ExternalProject_Add( catch
    PREFIX            ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY    https://github.com/philsquared/Catch.git
    UPDATE_COMMAND    ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
  )

  ExternalProject_Add( FakeIt
    PREFIX            ${CMAKE_BINARY_DIR}/FakeIt
    GIT_REPOSITORY    https://github.com/eranpeer/FakeIt.git
    UPDATE_COMMAND    ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
  )

  ExternalProject_Get_Property( catch source_dir )
  set( CATCH_INCLUDE_DIR "${source_dir}/include" )

  ExternalProject_Get_Property( FakeIt source_dir )
  set( FAKE_IT_INCLUDE_DIR "${source_dir}/single_header/standalone" )

  set( CM_ENABLE_MOCKING 1 )
endif( NOT DISABLE_TESTS )

set( LIBLIBW_PREFIX      "${PROJECT_BINARY_DIR}/liblibwireshark" )
set( LIBLIBW_INSTALL_DIR "${LIBLIBW_PREFIX}/install_dir" )
set( LIBLIBW_LIB_DIR     "${LIBLIBW_INSTALL_DIR}/lib" )

set( LIBLIBW_INCLUDE_DIRS "${LIBLIBW_INSTALL_DIR}/include/libwireshark" )

ExternalProject_Add( liblibwireshark
  SOURCE_DIR        "${PROJECT_SOURCE_DIR}/external/liblibwireshark"
  PREFIX            "${LIBLIBW_PREFIX}"
  CMAKE_ARGS
#    "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
#    "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}"
#    "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
#    "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
   "-DCMAKE_INSTALL_PREFIX=${LIBLIBW_INSTALL_DIR}"
   "-DENABLE_EXAMPLES=OFF"
   "-DWireshark_DIR=${Wireshark_DIR}"
   "-DPCAP_HINTS=${PCAP_HINTS}"
   "-DPCAP_ROOT_DIR=${PCAP_HINTS}"
#    -DDISABLE_TESTS=ON
#    -DENABLE_CODE_COVERAGE=OFF
  BUILD_BYPRODUCTS
    "${LIBLIBW_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}libwireshark${CMAKE_SHARED_LIBRARY_SUFFIX}"
    "${LIBLIBW_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}libwireshark${CMAKE_STATIC_LIBRARY_SUFFIX}"
)

add_library( libwireshark        SHARED IMPORTED )
add_library( libwireshark_static STATIC IMPORTED )
set_target_properties( libwireshark        PROPERTIES IMPORTED_LOCATION "${LIBLIBW_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}libwireshark${CMAKE_SHARED_LIBRARY_SUFFIX}" )
set_target_properties( libwireshark_static PROPERTIES IMPORTED_LOCATION "${LIBLIBW_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}libwireshark${CMAKE_STATIC_LIBRARY_SUFFIX}" )

####################
# Generate subdirs #
####################

# Write dummy files, so that they can be detected
file( WRITE "${PROJECT_SOURCE_DIR}/dataCollect/EPLEnum2Str.hpp" "" )
file( WRITE "${PROJECT_SOURCE_DIR}/dataCollect/EPLEnum2Str.cpp" "" )

new_project_library(
  PATH         "${PROJECT_SOURCE_DIR}/dataCollect"
  NAME         "epl_DataCollect"
  TEMPLATE     "${EPL_DC_TEMPLATES_DIR}/CMakeLists.lib.txt"
  DEPENDENCIES
    ${PYTHON_LIBRARIES}
    Threads::Threads ${FS_LIB}
    ${Wireshark_LIBRARY} ${GLIB_LIBRARIES} libwireshark_static caputils capchild ${PCAP_LIBRARIES}
    TinyXML2::TinyXML2
)

foreach( I IN ITEMS protocolValidator python timeSeries defaultFilter )
  new_project_library(
    PATH         "${PROJECT_SOURCE_DIR}/plugins/${I}"
    NAME         "eplp_${I}"
    TEMPLATE     "${EPL_DC_TEMPLATES_DIR}/CMakeLists.lib.txt"
    DEPENDENCIES epl_DataCollect_shared
  )
endforeach( I IN ITEMS protocolValidator python timeSeries )

new_project_library(
  PATH         "${PROJECT_SOURCE_DIR}/python"
  NAME         "eplp_pythonLibs"
  TEMPLATE     "${EPL_DC_TEMPLATES_DIR}/CMakeLists.python.txt"
  DEPENDENCIES epl_DataCollect_shared eplp_python_shared ${PYTHON_LIBRARIES}
)

if( NOT DISABLE_TESTS )
  new_project_executable(
    PATH         "${PROJECT_SOURCE_DIR}/tests"
    NAME         "tests"
    TEMPLATE     "${EPL_DC_TEMPLATES_DIR}/CMakeLists.exe.txt"
    DEPENDENCIES epl_DataCollect_shared eplp_python_shared eplp_timeSeries_shared eplp_defaultFilter_shared eplp_protocolValidator_shared
  )
endif( NOT DISABLE_TESTS )

##############################
# Update include directories #
##############################

include_directories( SYSTEM
  ${CATCH_INCLUDE_DIR}
  ${FAKE_IT_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${Wireshark_INCLUDE_DIR}
  ${LIBLIBW_INCLUDE_DIRS}
  ${GLIB_INCLUDE_DIRS}
  ${PLF_COLONY_DIR}
)

include_directories(
  ${EPL_DataCollect_LIB_INCLUDE_DIRECTORIES}
  ${EPL_DC_INCLUDES_DIR}
)

# Fix some compiler warnings
include_directories( SYSTEM ${PROJECT_SOURCE_DIR}/python )

if( NOT EXISTS "${EPL_DC_INCLUDES_DIR}" )
  file( MAKE_DIRECTORY "${EPL_DC_INCLUDES_DIR}" )
endif( NOT EXISTS "${EPL_DC_INCLUDES_DIR}" )

######################
# Add subdirectories #
######################

foreach( I IN LISTS EPL_DataCollect_SUBDIR_LIST )
  add_subdirectory( "${I}" )
endforeach( I IN LISTS EPL_DataCollect_SUBDIR_LIST )

if( NOT DISABLE_TESTS )
  add_dependencies( epl_DataCollect_obj FakeIt catch )
  add_custom_target( check COMMAND tests )
endif( NOT DISABLE_TESTS )

add_dependencies( epl_DataCollect_obj liblibwireshark )

##################
# Generate Files #
##################

configure_file( "${EPL_DC_TEMPLATES_DIR}/defines.in.hpp" "${EPL_DC_INCLUDES_DIR}/defines.hpp" @ONLY )

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Version.cmake"
    VERSION       ${CM_VERSION_MAJOR}.${CM_VERSION_MINOR}.${CM_VERSION_SUBMINOR}
    COMPATIBILITY SameMajorVersion
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Version.cmake
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
)

install( EXPORT ${PROJECT_NAME}Targets DESTINATION lib/cmake/${PROJECT_NAME} )

install(
  FILES "${EPL_DC_INCLUDES_DIR}/defines.hpp" "${EPL_DC_INCLUDES_DIR}/EPLEnums.h"
  DESTINATION include/eplViz
)

enum2str_generate(
  PATH       "${PROJECT_SOURCE_DIR}/dataCollect"
  CLASS_NAME "EPLEnum2Str"
  FUNC_NAME  "toStr"
  INCLUDES   "EPLEnums.h" "CaptureInstance.hpp" "XDDParser.hpp" "epan/ftypes/ftypes.h"
  INDENT_STR "  "
  NAMESPACE  "EPL_DataCollect"
  ENUMS
    "EvType" "ObjectType" "ObjectCategory" "ObjectDataType" "ObjectAccess" "ObjectPDOMapping" "AsyncSendPriority"
    "PacketType" "SDOCommandID" "ObjectClassType" "ASndServiceID" "SoARequestServiceID" "NMTState"
    "SDOResConnectionType" "SDOSndConnectionType" "SDOSegmentation" "NMTCommand"

    "CaptureInstance::CIstate" "XDDParser::ErrorCode"

    "ftenum"
  BLACKLIST
)

generate_format_command( format 3.9.1 )

#################
# Code Coverage #
#################

# if( ENABLE_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL GNU )
#   message( STATUS "COVERAGE ON" )
#   enable_testing()
#   add_test( NAME AllTests COMMAND tests )
#
#   include(Coveralls)
#
#   coveralls_setup(
#     "${${PROJECT_NAME}_ALL_UNASIGNED_CPP}"
#     ON
#   )
# endif( ENABLE_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL GNU )
