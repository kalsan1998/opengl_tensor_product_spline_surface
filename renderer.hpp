#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "bspline_surface.hpp"

class Renderer
{
public:
    Renderer(GLFWwindow *window, GLuint program);
    void SetDefaults();
    void Draw();
    void RotateWorld(float radians, int axis);
    void TranslateView(const glm::vec3 &val);
    void ZoomViewBy(float zoom);
    float GetZoomFactor() { return 5.0f / zoom; }
    bool IsInFreeMode() const { return free_mode; }
    void ToggleFreeMode() { free_mode = !free_mode; }

    float GetFrameBufferWidth() const { return float(frame_buffer_width); }
    float GetFrameBufferHeight() const { return float(frame_buffer_height); }
    float GeWindowWidth() const { return float(window_width); }
    float GeWindowHeight() const { return float(window_height); }

    void Resize(int width, int height);

private:
    GLFWwindow *window;
    GLuint program;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    int frame_buffer_width;
    int frame_buffer_height;
    int window_width;
    int window_height;

    float zoom;

    bool free_mode;

    BSplineSurface bspline_surface;
};

#endif