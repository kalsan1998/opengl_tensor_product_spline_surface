#include "renderer.hpp"

#include "glm/ext.hpp"

Renderer::Renderer(GLuint program) : program(program)
{
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    model = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(4.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    GLuint mvp = glGetUniformLocation(program, "model_view_projection");
    glUniformMatrix4fv(mvp, 1, GL_FALSE, &(projection * view * model)[0][0]);
}

void Renderer::Draw()
{
    GLuint mvp = glGetUniformLocation(program, "model_view_projection");
    glUniformMatrix4fv(mvp, 1, GL_FALSE, &(projection * view * model)[0][0]);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLfloat vertices[] = {
        -0.5f,
        -0.5f,
        0.0f,
        0.5f,
        0.5f,
        0.0f,
        0.0f,
        0.5f,
        0.0f,
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void Renderer::ProcessKeys(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        view = glm::translate(view, glm::vec3(0.0f, 0.05f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        view = glm::translate(view, glm::vec3(0.0f, -0.05f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        view = glm::translate(view, glm::vec3(0.05f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        view = glm::translate(view, glm::vec3(-0.05f, 0.0f, 0.0f));
    }
}