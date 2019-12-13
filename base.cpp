#include "base.hpp"

#include <iostream>

#include "glm/ext.hpp"

Base::Base(GLuint program) : program(program), zoom(5.0f), scale(1.0f), theta(0.0f), phi(0.0f), free_mode(false), sphere(program), bspline_surface(program)
{
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, zoom), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::mat4(1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    active_drawer = &bspline_surface;
}

void Base::Draw()
{
    GLuint model_location = glGetUniformLocation(program, "model");
    GLuint view_location = glGetUniformLocation(program, "view");
    GLuint projection_location = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &(model)[0][0]);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &(view)[0][0]);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &(projection)[0][0]);
    active_drawer->Draw();
}

void Base::ProcessKeysCallback(int key, int action)
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
    else if (key == GLFW_KEY_1)
    {
        active_drawer = &bspline_surface;
    }
    else if (key == GLFW_KEY_2)
    {
        active_drawer = &sphere;
    }
    else
    {
        active_drawer->ProcessKeys(key, action);
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