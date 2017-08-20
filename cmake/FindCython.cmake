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

# Set Cython_ROOT if not found

include(FindPackageHandleStandardArgs)

find_program( CYTHON_EXE
  NAMES cython3 cython
  HINTS ${Cython_ROOT}
)

if( CYTHON_EXE )
  execute_process(
    COMMAND "${CYTHON_EXE}" --version
    OUTPUT_VARIABLE CYTHON_VERSION_RAW
    ERROR_VARIABLE  CYTHON_VERSION_RAW
  )

  string( REGEX MATCH "[0-9]+\.[0-9]+(\.[0-9]+)?" CYTHON_VERSION "${CYTHON_VERSION_RAW}" )
endif( CYTHON_EXE )

find_package_handle_standard_args(
  Cython
  VERSION_VAR   CYTHON_VERSION
  REQUIRED_VARS CYTHON_EXE
)

if( Cython_FOUND )
  if( NOT TARGET Cython::Cython )
    add_executable( Cython::Cython IMPORTED GLOBAL )
    set_target_properties( Cython::Cython
      PROPERTIES
        IMPORTED_LOCATION "${CYTHON_EXE}"
    )
  endif( NOT TARGET Cython::Cython )
endif( Cython_FOUND )
