#include "renderer.hpp"

#include <iostream>

#include "glm/ext.hpp"

Renderer::Renderer(GLuint program) : program(program), bspline_surface(program)
{
    SetDefaults();
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

void Renderer::SetDefaults()
{
    zoom = 5.0f;
    free_mode = true;
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, zoom), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::mat4(1.0f);
}

void Renderer::RotateWorld(float radians, int axis)
{
    model = glm::rotate(model, radians, glm::vec3(model[0][axis], model[1][axis], model[2][axis]));
}

void Renderer::TranslateView(const glm::vec3 &val)
{
    view = glm::translate(view, val);
}

void Renderer::ZoomViewBy(float zoom_amount)
{
    zoom += zoom_amount;
    if (zoom < 0.2f)
    {
        zoom = 0.2f;
    }
    else
    {
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -zoom_amount));
    }
}

void Renderer::Draw()
{
    GLuint model_location = glGetUniformLocation(program, "model");
    GLuint view_location = glGetUniformLocation(program, "view");
    GLuint projection_location = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &(model)[0][0]);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &(view)[0][0]);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &(projection)[0][0]);
    bspline_surface.Draw();
}

void Renderer::Resize(int width, int height)

{
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}