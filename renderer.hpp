#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "bezier.hpp"
#include "bspline.hpp"
#include "sphere.hpp"

enum DrawObject
{
    DRAW_SPHERE,
    DRAW_BEZIER,
};

class Renderer
{
public:
    Renderer(GLuint program);
    void Draw();
    void ProcessKeysCallback(int key, int action);
    void MouseScroll(double y);

private:
    void DrawBezierCurve();
    void DrawSphere();

    GLuint program;
    GLuint color_location;

    DrawObject draw_object;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    float zoom;
    float theta;
    float phi;

    BSplineDrawer bspline;
    BezierDrawer bezier;
    SphereDrawer sphere;
};

#endif