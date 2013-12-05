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
	LLDLIBS = $(GLUT) -lGLU -lGL -lXmu -lXext -lX11 -lm
endif

CXX=g++
COMPILER_FLAGS= -g

INCLUDE= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) ${LLDLIBS}

TARGETS = FinalProject

SRCS = FinalProject.cpp trackball.cpp

default : $(TARGETS)

OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

FinalProject: FinalProject.o trackball.o
	$(CXX) $(COMPILER_FLAGS) $(LIBS) FinalProject.o trackball.o -o FinalProject

clean:
	rm -f $(OBJS) $(TARGETS) *~ *.o
