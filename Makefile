# "all" is name of the default target, running "make" without params would use it
all: main

# for C++, replace CC (c compiler) with CXX (c++ compiler) which is used as default linker
CC = $(CXX)

LDFLAGS = -L/usr/lib/ -lglfw3 -lGL -lglut -lGLEW

# tell which files should be used, .cpp -> .o make would do automatically
main: main.o shader_loader.o
    $(CC) $(LDFLAGS)
