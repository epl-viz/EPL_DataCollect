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

######################
# Set some variables #
######################

# Set the binary output directories
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake )

# Some useful variables
set( EPL_DC_TEMPLATES_DIR ${PROJECT_SOURCE_DIR}/cmake/templates )

################################
# Set up the build environment #
################################

run_git()

add_compiler( GNU MIN_VERSION "6.2"
               ALL      "-Wall -Wextra -Wno-comment -std=c++14"
            #   DEBUG    "-Werror"
               SANITIZE "-fsanitize=${SANITIZERS}"
               RELEASE  "-O3" )

add_compiler( Clang
               ALL      "-fvisibility=hidden -std=c++14 -Weverything -ftemplate-backtrace-limit=0"
                        "-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-float-equal"
                        "-Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-exit-time-destructors"
                        "-Wno-global-constructors -Wno-switch-enum -Wno-reserved-id-macro"
            #   DEBUG    "-Werror"
               SANITIZE "-fsanitize=${SANITIZERS}"
               RELEASE  "-O2" )

####################
# Generate subdirs #
####################

new_project_library(
  PATH     "${PROJECT_SOURCE_DIR}/dataCollect"
  NAME     "epl_DataCollect"
  TEMPLATE "${EPL_DC_TEMPLATES_DIR}/CMakeLists.lib.txt"
)

new_project_library(
  PATH     "${PROJECT_SOURCE_DIR}/disector/"
  NAME     "epl_disector"
  TEMPLATE "${EPL_DC_TEMPLATES_DIR}/CMakeLists.lib.txt"
)

new_project_library(
  PATH     "${PROJECT_SOURCE_DIR}/edsParser/"
  NAME     "epl_edsParser"
  TEMPLATE "${EPL_DC_TEMPLATES_DIR}/CMakeLists.lib.txt"
)

##################
# Generate Files #
##################

include_directories( ${EPL_DataCollect_LIB_INCLUDE_DIRECTORIES} )

if( NOT EXISTS ${PROJECT_SOURCE_DIR}/include )
  file( MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/include )
endif( NOT EXISTS ${PROJECT_SOURCE_DIR}/include )

configure_file( ${EPL_DC_TEMPLATES_DIR}/defines.in.hpp ${PROJECT_SOURCE_DIR}/include/defines.hpp @ONLY )

generate_format_command( format 3.9.1 )

######################
# Add subdirectories #
######################

foreach( I IN LISTS EPL_DataCollect_SUBDIR_LIST )
  add_subdirectory( "${I}" )
endforeach( I IN LISTS EPL_DataCollect_SUBDIR_LIST )



