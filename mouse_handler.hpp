#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

#include <glm/glm.hpp>

class Base;

class MouseHandler
{
public:
    MouseHandler(Base *base);
    void MouseMove(double x, double y);
    void MousePress(int button, int action);
    void MouseScroll(double y);
    glm::vec3 GetMouseRay() const;
    void Update();

private:
    Base *base;
    float mouse_x_pos;
    float mouse_y_pos;
    bool is_clicked = false;
};

#endif