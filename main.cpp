#include "shader_loader.hpp"
#include "renderer.hpp"
#include "mouse_handler.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

std::unique_ptr<MouseHandler> mouse_handler;
std::unique_ptr<Renderer> renderer;

void MouseScroll(GLFWwindow *window, double x, double y)
{
    if (mouse_handler)
        mouse_handler->MouseScroll(y);
}

void MousePress(GLFWwindow *window, int button, int action, int mods)
{
    if (mouse_handler)
        mouse_handler->MousePress(button, action);
}

void MouseMove(GLFWwindow *window, double x, double y)
{
    if (mouse_handler)
        mouse_handler->MouseMove(x, y);
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (renderer)
        renderer->KeyPress(key, action);
}

void Resize(GLFWwindow *window, int width, int height)
{
    if (renderer)
        renderer->Resize(width, height);
}

void SetUpImGui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
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

    SetUpImGui(window);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint shader_program = LoadShader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs");
    glUseProgram(shader_program);

    renderer = std::make_unique<Renderer>(shader_program);
    mouse_handler = std::make_unique<MouseHandler>(renderer.get());

    glfwSetScrollCallback(window, MouseScroll);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetWindowSizeCallback(window, Resize);
    glfwSetMouseButtonCallback(window, MousePress);
    glfwSetCursorPosCallback(window, MouseMove);

    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (renderer)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            renderer->GuiLogic(window);
            renderer->Draw();

            // Render dear imgui into screen
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}