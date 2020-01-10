#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

#include <glm/glm.hpp>

class Renderer;

class MouseHandler
{
public:
    MouseHandler(Renderer *renderer);
    void MouseMove(double x, double y);
    void MousePress(int button, int action);
    void MouseScroll(double y);
    glm::vec3 GetMouseRay() const;
    void Update();

private:
    Renderer *renderer;
    float mouse_x_pos;
    float mouse_y_pos;
    bool is_clicked = false;

    glm::vec3 ray;
};

#endif