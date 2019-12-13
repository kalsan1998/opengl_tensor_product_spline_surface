#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "drawer.hpp"
#include "bspline_surface.hpp"
#include "sphere.hpp"

class Base
{
public:
    Base(GLuint program);
    void Draw();
    void ProcessKeysCallback(int key, int action);
    void MouseScroll(double y);

private:
    GLuint program;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    float zoom;
    float scale;
    float theta;
    float phi;

    bool free_mode;

    Drawer *active_drawer;
    BSplineSurfaceDrawer bspline_surface;
    SphereDrawer sphere;
};

#endif