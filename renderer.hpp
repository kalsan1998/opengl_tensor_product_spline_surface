#include <glm/glm.hpp>
#include <GL/glut.h>

using namespace glm;
class Renderer
{
    Renderer();
    mat4 projection;
    mat4 view;
    mat4 model;
};