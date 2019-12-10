#include "renderer.hpp"

#include <iostream>

#include "glm/ext.hpp"

Renderer::Renderer(GLuint program) : draw_object(DRAW_SPHERE), program(program), zoom(5.0f), scale(1.0f), theta(0.0f), phi(0.0f), free_mode(false), sphere(program), bspline_surface(program)
{
    color_location = glGetUniformLocation(program, "uni_color");

    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, zoom), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::mat4(1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

void Renderer::Draw()
{
    GLuint model_location = glGetUniformLocation(program, "model");
    GLuint view_location = glGetUniformLocation(program, "view");
    GLuint projection_location = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &(model)[0][0]);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &(view)[0][0]);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &(projection)[0][0]);
    switch (draw_object)
    {
    case DRAW_SPHERE:
        sphere.Draw();
        break;
    case DRAW_BSPLINE_SURFACE:
        bspline_surface.Draw();
        break;
    default:
        break;
    }
}

void Renderer::ProcessKeysCallback(int key, int action)
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
    if (key == GLFW_KEY_UP)
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
    if (key == GLFW_KEY_LEFT)
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
    if (key == GLFW_KEY_RIGHT)
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
    if (key == GLFW_KEY_F)
    {
        free_mode = !free_mode;
    }
    if (draw_object == DRAW_SPHERE)
    {
        sphere.ProcessKeys(key, action);
    }
    if (draw_object == DRAW_BSPLINE_SURFACE)
    {
        bspline_surface.ProcessKeys(key, action);
    }
    if (key == GLFW_KEY_S)
    {
        draw_object = DRAW_SPHERE;
    }
    if (key == GLFW_KEY_B)
    {
        draw_object = DRAW_BSPLINE_SURFACE;
    }
}

void Renderer::MouseScroll(double y)
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