CC=gcc
CXX=g++

all: main
LDLIBS=-L/usr/lib/ -lglfw3 -lGL	-lglut -lGLEW

OBJS=main.o	shader_loader.o	renderer.o sphere.o bspline_surface.o
main: $(OBJS)
	$(CXX) -o main $(OBJS) $(LDLIBS)

main.o:	main.cpp shader_loader.hpp renderer.hpp
	$(CXX) -c main.cpp

shader_loader.o: shader_loader.cpp
	$(CXX) -c shader_loader.cpp

renderer.o: renderer.cpp sphere.hpp bspline_surface.hpp
	$(CXX) -c renderer.cpp

sphere.o: sphere.cpp
	$(CXX) -c sphere.cpp

bspline_surface.o: bspline_surface.cpp
	$(CXX) -c bspline_surface.cpp

clean:
	rm -f $(OBJS)

