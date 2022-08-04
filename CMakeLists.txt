################################################################################
# Copyright (c) 2022, Eero Talus
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
################################################################################
#
# @file CMakeLists.txt
#
# @description CMake configuration file
#
################################################################################

project(tiny-kiosk-browser C)
cmake_minimum_required(VERSION 3.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-4.0)

link_directories(
    ${GTK3_LIBRARY_DIRS}
    ${WEBKIT2_LIBRARY_DIRS}
)

set(SRC_DIR Src/)

set(SOURCES
    ${SRC_DIR}/Core/Src/main.c
    ${SRC_DIR}/Core/Src/browser.c
)

set(HEADERS
    ${SRC_DIR}
    ${GTK3_INCLUDE_DIRS}
    ${WEBKIT2_INCLUDE_DIRS}
)

set(LIBRARIES
    ${GTK3_LIBRARIES}
    ${WEBKIT2_LIBRARIES}
)

set(CFLAGS
    -g -Wall -Wextra -std=c11 -pedantic -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
    ${GTK3_CFLAGS}
    ${WEBKIT2_CFLAGS}
)

add_executable(tiny-kiosk-browser ${SOURCES})
target_include_directories(tiny-kiosk-browser PRIVATE ${HEADERS})
target_link_libraries(tiny-kiosk-browser PRIVATE ${LIBRARIES})
target_compile_options(tiny-kiosk-browser PRIVATE ${CFLAGS})
