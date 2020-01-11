#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include "base.hpp"

int main(int argc, char **argv)
{
    Base &base = Base::GetInstance();
    base.StartLoop();
}