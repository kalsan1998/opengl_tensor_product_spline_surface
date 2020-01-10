#include "mouse_handler.hpp"

MouseHandler::MouseHandler(Renderer *renderer) : renderer(renderer)
{
}

void MouseHandler::Update()
{
}

glm::vec3 MouseHandler::GetMouseRay() const
{
    return ray;
}

void MouseHandler::MouseMove(double x, double y)
{
    if (is_clicked)
    {
        float x_delta = x - mouse_x_pos;
        float y_delta = y - mouse_y_pos;

        // If in free mode then translate. Else rotate.
        if (renderer->IsInFreeMode())
        {
            renderer->TranslateView(glm::vec3(x_delta / 100.0f, -y_delta / 100.0f, 0.0f));
        }
        else
        {
            float theta_delta = glm::radians(x_delta);
            float phi_delta = glm::radians(y_delta);
            renderer->RotateWorld(theta_delta, 1);
            renderer->RotateWorld(phi_delta, 0);
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
            // std::cout << mouse_x_pos << " , " << mouse_y_pos << "\n";
            // glm::vec4 d = model * view * projection * glm::vec4(-0.25f, 0.25f, 0.0f, 1.0f);
            // std::cout << d[0] << "," << d[1] << "," << d[2] << "," << d[3] << "\n";
        }
    }
}

void MouseHandler::MouseScroll(double y)
{
    renderer->ZoomViewBy(0.2 * -y);
}