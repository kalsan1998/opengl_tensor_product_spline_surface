#include "renderer.hpp"

#include <iostream>

#include "glm/ext.hpp"

Renderer::Renderer(GLuint program) : program(program), zoom(5.0f), theta(0.0f), phi(0.0f)
{
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, zoom), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::mat4(1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    float control[12] = {
        -1.0f, -1.0f, 0.0f,
        -0.25f, 0.75f, 0.0f,
        0.5f, -0.25f, 0.0f,
        1.0f, 1.0f, 0.0f};
    bezier.SetControlPoints(control, 4);
}

void Renderer::Draw()
{
    GLuint model_location = glGetUniformLocation(program, "model");
    GLuint view_location = glGetUniformLocation(program, "view");
    GLuint projection_location = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &(model)[0][0]);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &(view)[0][0]);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &(projection)[0][0]);
    DrawBezierCurve();
}

void Renderer::ProcessKeys(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        phi += 0.1f;
        model = glm::rotate(model, 0.1f, glm::vec3(model[0][0], model[1][0], model[2][0]));
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        phi -= 0.1f;
        model = glm::rotate(model, -0.1f, glm::vec3(model[0][0], model[1][0], model[2][0]));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        theta -= 0.1f;
        model = glm::rotate(model, -0.1f, glm::vec3(model[0][1], model[1][1], model[2][1]));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        theta += 0.1f;
        model = glm::rotate(model, 0.1f, glm::vec3(model[0][1], model[1][1], model[2][1]));
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
}

void Renderer::DrawBezierCurve()
{
    GLuint color_location = glGetUniformLocation(program, "uni_color");

    float polygon_color[3] = {1.0f, 0.0f, 0.0f}; // Red
    glUniform3fv(color_location, 1, polygon_color);
    bezier.DrawControlPolygon();

    float curve_color[3] = {0.0f, 0.0f, 1.0f}; // Blue
    glUniform3fv(color_location, 1, curve_color);
    bezier.DrawBezierCurve();

    float point_color[3] = {1.0f, 0.0f, 0.0f}; // Red
    glUniform3fv(color_location, 1, point_color);
    bezier.DrawControlPoints();
}

void Renderer::DrawCube()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // GLuint colorbuffer;
    // glGenBuffers(1, &colorbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    // // 2nd attribute buffer : colors
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(
    //     2,        // attribute. No particular reason for 2, but must match the layout in the shader.
    //     3,        // size
    //     GL_FLOAT, // type
    //     GL_FALSE, // normalized?
    //     0,        // stride
    //     (void *)0 // array buffer offset
    // );

    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}