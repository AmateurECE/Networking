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
# LAST EDITED:	    04/04/2020
###

CC=/usr/bin/g++
CXX=/usr/bin/g++
CPPFLAGS=-g -Wall -Wextra -O0 --std=c++17 -I ../include
LDFLAGS=-lnetworking -L ../build
SRCS+=SimpleServer.cpp
SRCS+=SimpleClient.cpp
SRCS+=TlsServer.cpp
OBJS=${patsubst %.cpp,%.o,${SRCS}}

.PHONY: force

all: SimpleServer SimpleClient TlsServer

SimpleServer: SimpleServer.o
	$(CC) $^ $(LDFLAGS) -o $@

SimpleClient: SimpleClient.o
	$(CC) $^ $(LDFLAGS) -o $@

TlsServer: CPPFLAGS+= `pkg-config --cflags openssl`
TlsServer: LDFLAGS+= `pkg-config --libs openssl`
TlsServer: TlsServer.o
	$(CC) $^ $(LDFLAGS) -o $@

${OBJS}: force ${SRCS} ../build/libnetworking.a

../build/libnetworking.a: force
	cd .. && cmake -B build && make -C build

force:

###############################################################################
