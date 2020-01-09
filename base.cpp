#include "base.hpp"

#include <iostream>

#include "imgui/imgui.h"
#include "glm/ext.hpp"

Base::Base(GLuint program) : program(program), is_clicked(false), bspline_surface(program)
{
    SetDefaults();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

void Base::SetDefaults()
{
    zoom = 5.0f;
    scale = 1.0f;
    theta = 0.0f;
    phi = 0.0f;
    free_mode = true;
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, zoom), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::mat4(1.0f);
}

void Base::Draw()
{
    GLuint model_location = glGetUniformLocation(program, "model");
    GLuint view_location = glGetUniformLocation(program, "view");
    GLuint projection_location = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &(model)[0][0]);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &(view)[0][0]);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &(projection)[0][0]);
    bspline_surface.Draw();
}

void Base::KeyPress(int key, int action)
{
    if (action == GLFW_RELEASE)
        return;
    if (key == GLFW_KEY_DOWN)
    {
        if (free_mode)
        {
            view = glm::translate(view, glm::vec3(0.0f, 0.1f, 0.0f));
        }
        else
        {
            phi += 0.1f;
            model = glm::rotate(model, 0.1f, glm::vec3(model[0][0], model[1][0], model[2][0]));
        }
    }
    else if (key == GLFW_KEY_UP)
    {
        if (free_mode)
        {
            view = glm::translate(view, glm::vec3(0.0f, -0.1f, 0.0f));
        }
        else
        {
            phi -= 0.1f;
            model = glm::rotate(model, -0.1f, glm::vec3(model[0][0], model[1][0], model[2][0]));
        }
    }
    else if (key == GLFW_KEY_LEFT)
    {
        if (free_mode)
        {
            view = glm::translate(view, glm::vec3(0.1f, 0.0f, 0.0f));
        }
        else
        {
            theta -= 0.1f;
            model = glm::rotate(model, -0.1f, glm::vec3(model[0][1], model[1][1], model[2][1]));
        }
    }
    else if (key == GLFW_KEY_RIGHT)
    {
        if (free_mode)
        {
            view = glm::translate(view, glm::vec3(-0.1f, 0.0f, 0.0f));
        }
        else
        {
            theta += 0.1f;
            model = glm::rotate(model, 0.1f, glm::vec3(model[0][1], model[1][1], model[2][1]));
        }
    }
    else if (key == GLFW_KEY_TAB)
    {
        free_mode = !free_mode;
    }
    else if (key == GLFW_KEY_R)
    {
        SetDefaults();
    }
}

void Base::MouseMove(double x, double y)
{
    if (is_clicked)
    {
        float x_delta = x - mouse_x_pos;
        float y_delta = y - mouse_y_pos;

        // If in free mode then translate. Else rotate.
        if (free_mode)
        {
            view = glm::translate(view, glm::vec3(x_delta / 100.0f, -y_delta / 100.0f, 0.0f));
        }
        else
        {
            float theta_delta = glm::radians(x_delta);
            float phi_delta = glm::radians(y_delta);
            theta += theta_delta;
            phi += phi_delta;
            model = glm::rotate(model, theta_delta, glm::vec3(model[0][1], model[1][1], model[2][1]));
            model = glm::rotate(model, phi_delta, glm::vec3(model[0][0], model[1][0], model[2][0]));
        }
    }
    mouse_x_pos = x;
    mouse_y_pos = y;
}

void Base::MousePress(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        is_clicked = (GLFW_PRESS == action);
    }
}

void Base::MouseScroll(double y)
{
    zoom += 0.2f * -y;
    if (zoom < 0.2f)
    {
        zoom = 0.2f;
    }
    else
    {
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.2f * y));
    }
    scale = std::max(5.0f / zoom, 1.0f);
}

void Base::Resize(int width, int height)

{
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void ToolTipHelper(const char *text)
{
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void Base::GuiLogic(GLFWwindow *window)
{
    static bool show_window(true);
    ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Begin("##window", &show_window, windowFlags);
    if (ImGui::Button("Reset Camera"))
    {
        SetDefaults();
    }
    ToolTipHelper("R");
    ImGui::Checkbox("Free Roam", &free_mode);
    ToolTipHelper("TAB");
    ImGui::Text("Zoom Factor: %.1f", 5.0f / zoom);
    ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
    ImGui::End();
}
