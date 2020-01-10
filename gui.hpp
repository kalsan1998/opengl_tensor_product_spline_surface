#ifndef GUI_H
#define GUI_H

class GLFWwindow;
class Renderer;

class Gui
{
public:
    Gui(GLFWwindow *window, Renderer *renderer);
    void HandleLogic();

private:
    GLFWwindow *window;
    Renderer *renderer;
};

#endif