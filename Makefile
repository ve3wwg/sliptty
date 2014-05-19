######################################################################
#  sliptty/Makefile - Generic POSIX Makefile
#  Warren Gay ve3wwg	Mon May 19 09:19:25 2014
######################################################################

PREFIX	= /usr/local

ARCH	= 
DEFNS	= 
STD	= -std=gnu++0x

GCOPTS	= -Wall -g -Os 
GLIBS	= -lstdc++ -lc -lm 

INCL	= $(PREFIX)/include -I.

CPPFLAGS = $(GOPTS) $(INCL) $(OPTIONS) $(INCL) $(DEFNS)
CXXFLAGS = $(STD) $(GCOPTS) $(CPPFLAGS)

CXX	= $(ARCH)c++
OBJSIZE	= $(ARCH)size
OBJCOPY	= $(ARCH)objcopy
AR	= $(ARCH)ar
MAKE	= gmake

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $*.o

CPP_FILES 	:= $(wildcard *.cpp)
OBJS 		:= $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all:	libsliptty.a

sliptty.o: sliptty.cpp sliptty.hpp Makefile

libsliptty.a: sliptty.o
	$(AR) r libsliptty.a sliptty.o

testdb: testdb.o libsliptty.a
	$(CXX) testdb.o -o testdb -L. -lsliptty -L$(PREFIX)/lib -lsqlite3

check:	testdb
	./testdb	# Creates test.db if non-existing
	./testdb	# Re-opens test.db
	rm -f test.db

install: libsliptty.a
	install -m 444 libsliptty.a $(PREFIX)/lib 
	install -m 444 sliptty.hpp $(PREFIX)/include

uninstall:
	rm -f $(PREFIX)/lib/libsliptty.a $(PREFIX)/include/sliptty.hpp

clean:
	rm -f *.o testdb

distclean: clean
	rm -f libsliptty.a testdb errs.t

# End sliptty/Makefile
