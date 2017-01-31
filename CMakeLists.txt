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

option( DISABLE_TESTS        "Disables all tests"          OFF )
option( ENABLE_CODE_COVERAGE "Enables code coverage tests" OFF )

list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" )

# Some useful variables
set( EPL_DC_TEMPLATES_DIR "${PROJECT_SOURCE_DIR}/cmake/templates" )
set( EPL_DC_INCLUDES_DIR  "${PROJECT_SOURCE_DIR}/include" )
set( PLF_COLONY_DIR       "${PROJECT_SOURCE_DIR}/external/plf_colony/" )

set( CM_ENABLE_MOCKING 0 )

################################
# Set up the build environment #
################################

if( ENABLE_CODE_COVERAGE )
  set( COVERAGE_COMPILER_FLAG "--coverage" )
endif( ENABLE_CODE_COVERAGE )

run_git()

add_compiler(
  GNU MIN_VERSION "6.2"
  ALL        "-Wall -Wextra -Wfloat-equal -Wshadow -Wno-unknown-pragmas"
  DEBUG      "-O0 -Werror" ${COVERAGE_COMPILER_FLAG}
  RELEASE    "-O3"

  C_ALL      "-Wall -Wextra -Wfloat-equal -Wshadow -Wno-unknown-pragmas"
  C_DEBUG    "-O0 -Werror" ${COVERAGE_COMPILER_FLAG}
  C_RELEASE  "-O3"

  SANITIZE   "-fsanitize=${SANITIZERS}"
)

add_compiler(
  Clang MIN_VERSION 3.9
  ALL        "-Weverything -ftemplate-backtrace-limit=0"
             "-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded"
             "-Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-exit-time-destructors"
             "-Wno-global-constructors -Wno-switch-enum -Wno-reserved-id-macro"
             "-Wno-documentation-unknown-command -Wno-unknown-pragmas"
             "-Wno-disabled-macro-expansion"
  DEBUG      "-Werror"
  RELEASE    "-O2"

  C_ALL      "-Weverything -ftemplate-backtrace-limit=0"
             "-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-float-equal"
             "-Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-exit-time-destructors"
             "-Wno-global-constructors -Wno-switch-enum -Wno-reserved-id-macro"
             "-Wno-documentation-unknown-command -Wno-unknown-pragmas"
             "-Wno-disabled-macro-expansion"
  C_DEBUG    "-Werror"
  C_RELEASE  "-O2"

  SANITIZE "-fsanitize=${SANITIZERS}"
)

######################
# Check Dependencies #
######################

find_package( Git              REQUIRED )
find_package( PythonInterp 3.6 REQUIRED ) # Exec python with PYTHON_EXECUTABLE
find_package( PythonLibs   3.6 REQUIRED )
find_package( PkgConfig        REQUIRED )
find_package( Threads          REQUIRED )
pkg_search_module( LIBWIRESHARK REQUIRED wireshark )

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

  if( CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU )
    string( APPEND CMAKE_CXX_FLAGS " -isystem ${FAKE_IT_INCLUDE_DIR} " )
  endif( CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU )

  set( CM_ENABLE_MOCKING 1 )
endif( NOT DISABLE_TESTS )

if( CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU )
  string( APPEND CMAKE_CXX_FLAGS " -isystem ${PLF_COLONY_DIR} " )
endif( CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU )

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
  DEPENDENCIES epl_dissector_shared epl_edsParser_shared ${PYTHON_LIBRARIES} Threads::Threads
)

new_project_library(
  PATH     "${PROJECT_SOURCE_DIR}/dissector"
  NAME     "epl_dissector"
  TEMPLATE "${EPL_DC_TEMPLATES_DIR}/CMakeLists.lib.txt"
)

new_project_library(
  PATH     "${PROJECT_SOURCE_DIR}/edsParser"
  NAME     "epl_edsParser"
  TEMPLATE "${EPL_DC_TEMPLATES_DIR}/CMakeLists.lib.txt"
)

foreach( I IN ITEMS protocolValidator python timeSeries )
  new_project_library(
    PATH         "${PROJECT_SOURCE_DIR}/plugins/${I}"
    NAME         "eplp_${I}"
    TEMPLATE     "${EPL_DC_TEMPLATES_DIR}/CMakeLists.lib.txt"
    DEPENDENCIES epl_DataCollect_shared
  )
endforeach( I IN ITEMS protocolValidator python timeSeries )

if( NOT DISABLE_TESTS )
  new_project_executable(
    PATH         "${PROJECT_SOURCE_DIR}/tests"
    NAME         "tests"
    TEMPLATE     "${EPL_DC_TEMPLATES_DIR}/CMakeLists.exe.txt"
    DEPENDENCIES epl_DataCollect_shared epl_dissector_shared epl_edsParser_shared
  )
endif( NOT DISABLE_TESTS )

##############################
# Update include directories #
##############################

include_directories(
  ${EPL_DataCollect_LIB_INCLUDE_DIRECTORIES}
  ${CATCH_INCLUDE_DIR}
  ${FAKE_IT_INCLUDE_DIR}
  ${EPL_DC_INCLUDES_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${LIBWIRESHARK_INCLUDE_DIRS}
  ${PLF_COLONY_DIR}
)

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
  add_custom_target( check COMMAND tests )
endif( NOT DISABLE_TESTS )

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
  INCLUDES   "EPLEnums.h" "CaptureInstance.hpp"
  NAMESPACE  "EPL_DataCollect"
  ENUMS
    "NodeStatus" "EvType" "ObjectType" "ObjectCategory" "ObjectDataType" "ObjectAccess" "ObjectPDOMapping"
    "PacketType" "CommandID" "ObjectClassType"

    "CaptureInstance::CIstate"
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
