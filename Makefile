CC=gcc
CXX=g++

all: main
LDLIBS=-L/usr/lib/ -lglfw3 -lGL	-lglut -lGLEW

OBJS=main.o	shader_loader.o	renderer.o sphere.o bezier.o bspline.o
main: $(OBJS)
	$(CXX) -o main $(OBJS) $(LDLIBS)

main.o:	main.cpp shader_loader.hpp renderer.hpp
	$(CXX) -c main.cpp

shader_loader.o: shader_loader.cpp
	$(CXX) -c shader_loader.cpp

renderer.o: renderer.cpp sphere.hpp bezier.hpp bspline.hpp
	$(CXX) -c renderer.cpp

sphere.o: sphere.cpp
	$(CXX) -c sphere.cpp

bezier.o: bezier.cpp
	$(CXX) -c bezier.cpp

bspline.o: bspline.cpp
	$(CXX) -c bspline.cpp

clean:
	rm -f $(OBJS)

