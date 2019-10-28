#******************************************************************************#
#   Copyright 2018 AMIQ Consulting s.r.l.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
#******************************************************************************#
#   Original Authors: Teodor Vasilache and Dragos Dospinescu,
#                     AMIQ Consulting s.r.l. (contributors@amiq.com)
#
#               Date: 2018-Feb-20
#******************************************************************************#
CC = g++-4.8
LD = g++-4.8
AR = gcc-ar-4.8

EXEC = libsimple_bus.a 

INCLUDE_SYSTEMC = -I${SYSTEMC_HOME}/include

INCLUDES = $(INCLUDE_SYSTEMC) -Iinclude
CFLAGS = -g -O0 -c -std=c++11
LDPATH = export LD_LIBRARY_PATH=${SYSTEMC_HOME}/lib-linux64
SRCFILES = $(wildcard src/*.cpp)

OBJFILES = $(patsubst src/%.cpp, obj/%.o, ${SRCFILES})

_dummy := $(shell mkdir -p obj)

all: main

main: $(OBJFILES)
	$(AR) -rcs $(EXEC) $(OBJFILES)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $^ -o $@

clean:
	rm -rf obj $(EXEC)
