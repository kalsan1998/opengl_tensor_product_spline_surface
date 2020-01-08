#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "bspline_surface.hpp"

class Base
{
public:
    Base(GLuint program);
    void Draw();
    void KeyPress(int key, int action);
    void MouseMove(double x, double y);
    void MousePress(int button, int action);
    void MouseScroll(double y);
    void Resize(int width, int height);
    void GuiLogic(GLFWwindow *window);

private:
    void SetDefaults();

    GLuint program;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    float zoom;
    float scale;
    float theta;
    float phi;

    float mouse_x_pos;
    float mouse_y_pos;

    bool free_mode;
    bool is_clicked;

    BSplineSurface bspline_surface;
};

#endif