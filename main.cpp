#include "shader_loader.hpp"
#include "base.hpp"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

std::unique_ptr<Base> base;
void MouseScroll(GLFWwindow *window, double x, double y)
{
    if (base)
        base->MouseScroll(y);
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (base)
        base->ProcessKeysCallback(key, action);
}

void Resize(GLFWwindow *window, int width, int height)
{
    if (base)
        base->Resize(width, height);
}

int main(int argc, char **argv)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glewExperimental = true; // Needed for core profile
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    // Open a window and create its OpenGL context
    GLFWwindow *window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(1024, 768, "Code!", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }

    // Check Fragment Shader
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental = true;        // Needed in core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint shader_program = LoadShader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs");
    glUseProgram(shader_program);
    base = std::make_unique<Base>(shader_program);
    glfwSetScrollCallback(window, MouseScroll);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetWindowSizeCallback(window, Resize);
    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (base)
        {
            base->Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            return 0;
    }
}