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

# string( REGEX REPLACE "-Werror" "" CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}" )
# string( REGEX REPLACE "-Werror" "" CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}" )
# string( REGEX REPLACE "-Werror" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" )

set( CMAKE_CXX_FLAGS         ${CMAKE_CXX_FLAGS}         PARENT_SCOPE )
set( CMAKE_CXX_FLAGS_DEBUG   ${CMAKE_CXX_FLAGS_DEBUG}   PARENT_SCOPE )
set( CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} PARENT_SCOPE )

set( CYTHON_FILES_DIR "${CMAKE_CURRENT_LIST_DIR}/cython" )

file(
  GLOB PYTHON_FILES
  LIST_DIRECTORIES OFF
  RELATIVE ${CYTHON_FILES_DIR}
  "${CYTHON_FILES_DIR}/*.pyx"
)

foreach( I IN LISTS PYTHON_FILES )
  string( APPEND CM_PYTHON_FILES "\n   \${CMAKE_CURRENT_LIST_DIR}/cython/${I}" )
endforeach( I IN LISTS PYTHON_FILES )

set( CM_PYTHON_FILES "${CM_PYTHON_FILES}" PARENT_SCOPE )

# add_library( pTest SHARED ${CMAKE_CURRENT_LIST_DIR}/Cycle.cpp )
