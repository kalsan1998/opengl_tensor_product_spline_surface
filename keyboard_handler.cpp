#include "keyboard_handler.hpp"

#include "renderer.hpp"

KeyboardHandler::KeyboardHandler(Renderer *renderer) : renderer(renderer)
{
}

void KeyboardHandler::KeyPress(int key, int action)
{
    if (action == GLFW_RELEASE)
        return;

    if (key == GLFW_KEY_DOWN)
    {
        if (renderer->IsInFreeMode())
        {
            renderer->TranslateView(glm::vec3(0.0f, -0.1f, 0.0f));
        }
        else
        {
            renderer->RotateWorld(0.1f, 0);
        }
    }
    else if (key == GLFW_KEY_UP)
    {
        if (renderer->IsInFreeMode())
        {
            renderer->TranslateView(glm::vec3(0.0f, 0.1f, 0.0f));
        }
        else
        {
            renderer->RotateWorld(-0.1f, 0);
        }
    }
    else if (key == GLFW_KEY_LEFT)
    {
        if (renderer->IsInFreeMode())
        {
            renderer->TranslateView(glm::vec3(-0.1f, 0.0f, 0.0f));
        }
        else
        {
            renderer->RotateWorld(-0.1f, 1);
        }
    }
    else if (key == GLFW_KEY_RIGHT)
    {
        if (renderer->IsInFreeMode())
        {
            renderer->TranslateView(glm::vec3(0.1f, 0.0f, 0.0f));
        }
        else
        {
            renderer->RotateWorld(0.1f, 1);
        }
    }
    else if (key == GLFW_KEY_TAB)
    {
        renderer->ToggleFreeMode();
    }
    else if (key == GLFW_KEY_R)
    {
        renderer->SetDefaults();
    }
}