#!smake
#  @@ Makefile for CMSC 435/634 - Project 1: Simple scene
#  @@ Jian Chen (jichen@umbc.edu)
#

MACHINE= $(shell uname -s)

ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa
	LLDLIBS=
else
	OPENGL_INC= -I/usr/X11R6/include
	OPENGL_LIB= -L/usr/lib64 -lglut -lGL -lGLU
	LLDLIBS = $(GLUT) -lGLU -lGL -lXext -lX11 -lm
endif

CXX=g++
COMPILER_FLAGS= -g

INCLUDE= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) ${LLDLIBS}

TARGETS = FinalProject

SRCS = FinalProject.cpp CLUSTER_STRUCT.cpp trackball.c
SRCS2 = trackball.c

default : $(TARGETS)

OBJS = $(SRCS:.cpp=.o)
OBJS2 = $(SRCS2:.c=.o)

%.o: %.cpp
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

%.o: %.c
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

FinalProject: FinalProject.o trackball.o CLUSTER_STRUCT.o
	$(CXX) $(COMPILER_FLAGS) $(LIBS) FinalProject.o trackball.o CLUSTER_STRUCT.o -o FinalProject

clean:
	rm -f $(OBJS) $(TARGETS) *~ *.o
