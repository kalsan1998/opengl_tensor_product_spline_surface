CC=gcc
CXX=g++
CXXFLAGS=-Wall -g

all: main
LDLIBS=-L/usr/lib/ -lglfw3 -lGL -lGLEW

OBJS=main.o	shader_loader.o	base.o sphere.o bspline_surface.o imgui.o imgui_impl_opengl3.o imgui_impl_glfw.o imgui_draw.o imgui_widgets.o
main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o main $(OBJS) $(LDLIBS)

main.o:	main.cpp shader_loader.hpp base.hpp imgui/imgui.h imgui/imgui_impl_opengl3.h imgui/imgui_impl_glfw.h
	$(CXX) $(CXXFLAGS) -c main.cpp

shader_loader.o: shader_loader.cpp
	$(CXX) $(CXXFLAGS) -c shader_loader.cpp

base.o: base.cpp sphere.hpp bspline_surface.hpp imgui/imgui.h
	$(CXX) $(CXXFLAGS) -c base.cpp

sphere.o: sphere.cpp
	$(CXX) $(CXXFLAGS) -c sphere.cpp

bspline_surface.o: bspline_surface.cpp
	$(CXX) $(CXXFLAGS) -c bspline_surface.cpp

imgui.o: imgui/imgui.cpp imgui/imgui_internal.h
	$(CXX) $(CXXFLAGS) -c imgui/imgui.cpp

imgui_draw.o: imgui/imgui_draw.cpp imgui/imgui_internal.h
	$(CXX) $(CXXFLAGS) -c imgui/imgui_draw.cpp

imgui_widgets.o: imgui/imgui_widgets.cpp imgui/imgui_internal.h
	$(CXX) $(CXXFLAGS) -c imgui/imgui_widgets.cpp

imgui_impl_opengl3.o: imgui/imgui_impl_opengl3.cpp imgui/imgui.h
	$(CXX) $(CXXFLAGS) -c imgui/imgui_impl_opengl3.cpp

imgui_impl_glfw.o: imgui/imgui_impl_glfw.cpp imgui/imgui.h
	$(CXX) $(CXXFLAGS) -c imgui/imgui_impl_glfw.cpp

clean:
	rm -f $(OBJS)

