#include "mouse_handler.hpp"

#include "base.hpp"

#include <iostream>

MouseHandler::MouseHandler(Base *base) : base(base)
{
}

void MouseHandler::MouseMove(double x, double y)
{
    if (is_clicked)
    {
        float x_delta = x - mouse_x_pos;
        float y_delta = y - mouse_y_pos;

        // If in free mode then translate. Else rotate.
        if (base->IsInFreeMode())
        {
            base->TranslateView(glm::vec3(x_delta / 100.0f, -y_delta / 100.0f, 0.0f));
        }
        else
        {
            float theta_delta = glm::radians(x_delta);
            float phi_delta = glm::radians(y_delta);
            base->RotateWorld(theta_delta, 1);
            base->RotateWorld(phi_delta, 0);
        }
    }
    mouse_x_pos = x;
    mouse_y_pos = y;
}

void MouseHandler::MousePress(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        is_clicked = (GLFW_PRESS == action);
        if (is_clicked)
        {
            // GL mouse coordinates.
            float x_pos = mouse_x_pos;
            float y_pos = base->GeWindowHeight() - mouse_y_pos;
            x_pos *= base->GeWindowWidth() / base->GetFrameBufferWidth();
            y_pos *= base->GeWindowHeight() / base->GetFrameBufferHeight();

            std::cout << base->GeWindowWidth() << " " << y_pos << std::endl;
        }
    }
}

void MouseHandler::MouseScroll(double y)
{
    base->ZoomViewBy(0.2 * -y);
}