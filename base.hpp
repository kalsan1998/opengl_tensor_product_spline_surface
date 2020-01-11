#ifndef BASE_H
#define BASE_H

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "bspline_surface.hpp"
#include "shader_loader.hpp"
#include "base.hpp"
#include "mouse_handler.hpp"
#include "keyboard_handler.hpp"
#include "gui.hpp"

class Base
{
public:
    static Base &GetInstance()
    {
        static Base base;
        return base;
    }
    ~Base();
    void StartLoop();

    void SetDefaults();
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

private:
    Base();
    Base(Base const&) = delete;
    void operator=(Base const&) = delete;
    void Draw();
    void Resize(int width, int height);

    static void MousePressCallback(GLFWwindow *window, int button, int action, int mods);
    static void MouseMoveCallback(GLFWwindow *window, double x, double y);
    static void MouseScrollCallback(GLFWwindow *window, double x, double y);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void ResizeCallback(GLFWwindow *window, int width, int height);

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

    std::unique_ptr<BSplineSurface> bspline_surface;
    std::unique_ptr<MouseHandler> mouse_handler;
    std::unique_ptr<KeyboardHandler> keyboard_handler;
    std::unique_ptr<Gui> gui;
};

#endif