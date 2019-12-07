#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "bezier.hpp"
#include "sphere.hpp"

class Renderer
{
public:
    Renderer(GLuint program);
    void Draw();
    void ProcessKeys(GLFWwindow *window);
    void MouseScroll(double y);

private:
    void DrawBezierCurve();
    void DrawSphere();

    GLuint program;
    GLuint color_location;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    float zoom;
    float theta;
    float phi;

    BezierDrawer bezier;
    SphereDrawer sphere;
};

#endif