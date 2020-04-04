###############################################################################
# NAME:		    Makefile
#
# AUTHOR:	    Ethan D. Twardy <edtwardy@mtu.edu>
#
# DESCRIPTION:	    Makefile for the examples. These examples don't warrant a
#		    CMake file.
#
# CREATED:	    04/03/2020
#
# LAST EDITED:	    04/03/2020
###

CC=/usr/bin/g++
CXX=/usr/bin/g++
CPPFLAGS=-g -Wall -Wextra -O0 --std=c++17 -I ../include
LDFLAGS=-lnetworking -L ../build
SRCS+=SimpleServer.cpp
SRCS+=SimpleClient.cpp
OBJS=${patsubst %.cpp,%.o,${SRCS}}

.PHONY: force

all: SimpleServer SimpleClient

SimpleServer: SimpleServer.o

SimpleClient: SimpleClient.o

${OBJS}: force ${SRCS}

force:

###############################################################################
