#include "keyboard_handler.hpp"

#include "base.hpp"

KeyboardHandler::KeyboardHandler(Base *base) : base(base)
{
}

void KeyboardHandler::KeyPress(int key, int action)
{
    if (action == GLFW_RELEASE)
        return;

    if (key == GLFW_KEY_DOWN)
    {
        if (base->IsInFreeMode())
        {
            base->TranslateView(glm::vec3(0.0f, -0.1f, 0.0f));
        }
        else
        {
            base->RotateWorld(0.1f, 0);
        }
    }
    else if (key == GLFW_KEY_UP)
    {
        if (base->IsInFreeMode())
        {
            base->TranslateView(glm::vec3(0.0f, 0.1f, 0.0f));
        }
        else
        {
            base->RotateWorld(-0.1f, 0);
        }
    }
    else if (key == GLFW_KEY_LEFT)
    {
        if (base->IsInFreeMode())
        {
            base->TranslateView(glm::vec3(-0.1f, 0.0f, 0.0f));
        }
        else
        {
            base->RotateWorld(-0.1f, 1);
        }
    }
    else if (key == GLFW_KEY_RIGHT)
    {
        if (base->IsInFreeMode())
        {
            base->TranslateView(glm::vec3(0.1f, 0.0f, 0.0f));
        }
        else
        {
            base->RotateWorld(0.1f, 1);
        }
    }
    else if (key == GLFW_KEY_TAB)
    {
        base->ToggleFreeMode();
    }
    else if (key == GLFW_KEY_R)
    {
        base->SetDefaults();
    }
}