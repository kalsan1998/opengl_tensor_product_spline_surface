#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
using namespace glm;

extern const char *kVertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 pos;
void main() {
    gl_Position = vec4(pos, 1.0);
}
)";
extern const char *kFragmentShaderSource = R"(
#version 330 core
out vec3 color;
void main() {
  color = vec3(1.0,0.0,0.0);
}
)";
