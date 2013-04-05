#
# Makefile for P2
#
# Instructions:
#
# 1. Add only .cc files to the line beginning with SRCS = main.cc.
#    Each file should be separated with spaces, not commas.
# 2. To build the dependency list, type 'make depend'.  You'll need to
#    do this whenever you add a new .cc or .h file to your project.
# 3. To build the project, type 'make'.
# 4. To start over, type 'make clean', which does not clean or update
#    the dependency list.
#
# N.B. If you're using files with .cpp extensions, then you need to search
# and replace cc with cpp.
#

CC = g++
CFLAGS = -Wall -g
SRCS = main.cc 
OBJS = ${SRCS:.cc=.o}

a.out: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)

$(OBJS):
	$(CC) $(CFLAGS) -c $*.cc

depend: Makefile.dep
	$(CC) -MM $(SRCS) > Makefile.dep

Makefile.dep:
	touch Makefile.dep

clean:
	rm -f $(OBJS) a.out core

include Makefile.dep

