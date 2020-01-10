#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "bspline_surface.hpp"

class Renderer
{
public:
    Renderer(GLuint program);
    void Draw();
    void RotateWorld(float radians, int axis);
    void TranslateView(const glm::vec3 &val);
    void ZoomViewBy(float zoom);
    bool IsInFreeMode() const { return free_mode; }

    void KeyPress(int key, int action);
    void Resize(int width, int height);
    void GuiLogic(GLFWwindow *window);

private:
    void SetDefaults();

    GLuint program;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    float zoom;

    bool free_mode;

    BSplineSurface bspline_surface;
};

#endif