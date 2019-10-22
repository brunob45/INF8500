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

include crave.makefile

CC = g++-4.8
LD = g++-4.8

EXEC = main 

LIBS = $(crave_LIBRARIES)
RPATH = $(crave_RPATH)

INCLUDE_SYSTEMC = -I${SYSTEMC_HOME}/include
INCLUDE_FC4SC = -I${FC4SC_HOME}/includes

INCLUDES = $(INCLUDE_SYSTEMC) $(INCLUDE_FC4SC) $(crave_INCLUDES) 
CFLAGS =  -g -O0 -c -std=c++11
DEFINES = -DFC4SC_NO_THROW
LDFLAGS = -L${SYSTEMC_HOME}/lib-linux64 -lsystemc 
LDPATH = export LD_LIBRARY_PATH="/home/guy/crave/deps/lingeling-ayv-86bf266-140429/lib":"/home/guy/crave/deps/boolector-2.2.0/lib":"/home/guy/crave/deps/boost-1_55_0-fs/lib":"/home/guy/crave/deps/glog-git-0.3.3/lib":"/home/guy/crave/deps/cudd-3.0.0/lib":"/home/guy/crave/deps/minisat-git/lib":"/home/guy/crave/build/root/lib":"/home/guy/crave/deps/systemc-2.3.1/lib-linux64"
SRCFILES = $(wildcard src/*.cpp)
HFILES = $(wildcard src/*.h)

OBJFILES = $(patsubst %.cpp, obj/%.o, ${SRCFILES})

_dummy := $(shell mkdir -p obj/src)

all: main

main: $(OBJFILES)
	$(LD) $(OBJFILES) -o $(EXEC) $(LIBS) $(RPATH) $(LDFLAGS) -lpthread

obj/%.o: %.cpp $(HFILES)
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $< -o $@

run: main
	$(LDPATH) && ./$(EXEC)

clean:
	rm -rf obj $(EXEC)

.PHONY: all clean run