#ifndef GUI_H
#define GUI_H

class GLFWwindow;
class Base;

class Gui
{
public:
    Gui(GLFWwindow *window, Base *base);
    void HandleLogic();

private:
    GLFWwindow *window;
    Base *base;
};

#endif