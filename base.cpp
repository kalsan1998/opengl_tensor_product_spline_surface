#include "base.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "glm/ext.hpp"

#include "shader_loader.hpp"

Base::~Base()
{
}

Base::Base()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glewExperimental = true; // Needed for core profile
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }
    window = glfwCreateWindow(1024, 768, "CS488 Project", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
    }

    // Set input handlers.
    gui = std::make_unique<Gui>(window, this);
    keyboard_handler = std::make_unique<KeyboardHandler>(this);
    mouse_handler = std::make_unique<MouseHandler>(this);
    glfwSetScrollCallback(window, MouseScrollCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetWindowSizeCallback(window, ResizeCallback);
    glfwSetMouseButtonCallback(window, MousePressCallback);
    glfwSetCursorPosCallback(window, MouseMoveCallback);

    // Set up ImGui.
    ImGui::CreateContext();
    // Setup Platform/Base bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Load shaders.
    program = LoadShader("shaders/vertex_shader.vs",
                         "shaders/fragment_shader.fs");
    glUseProgram(program);
}

void Base::MouseScrollCallback(GLFWwindow *win, double x, double y)
{
    GetInstance().mouse_handler->MouseScroll(y);
}

void Base::MousePressCallback(GLFWwindow *win, int button, int action, int mods)
{
    GetInstance().mouse_handler->MousePress(button, action);
}

void Base::MouseMoveCallback(GLFWwindow *win, double x, double y)
{
    GetInstance().mouse_handler->MouseMove(x, y);
}

void Base::KeyCallback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
    GetInstance().keyboard_handler->KeyPress(key, action);
}

void Base::ResizeCallback(GLFWwindow *window, int width, int height)
{
    GetInstance().Resize(width, height);
}

void Base::StartLoop()
{
    SetDefaults();
    glfwGetFramebufferSize(window, &frame_buffer_width, &frame_buffer_height);
    glfwGetWindowSize(window, &window_width, &window_height);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    bspline_surface = std::make_unique<BSplineSurface>(program);
    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        gui->HandleLogic();
        Draw();

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Base::SetDefaults()
{
    zoom = 5.0f;
    free_mode = true;
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, zoom), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::mat4(1.0f);
}

void Base::RotateWorld(float radians, int axis)
{
    model = glm::rotate(model, radians, glm::vec3(model[0][axis], model[1][axis], model[2][axis]));
}

void Base::TranslateView(const glm::vec3 &val)
{
    view = glm::translate(view, val);
}

void Base::ZoomViewBy(float zoom_amount)
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

void Base::Draw()
{
    GLuint model_location = glGetUniformLocation(program, "model");
    GLuint view_location = glGetUniformLocation(program, "view");
    GLuint projection_location = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &(model)[0][0]);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &(view)[0][0]);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &(projection)[0][0]);
    bspline_surface->Draw();
}

void Base::Resize(int width, int height)

{
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glfwGetFramebufferSize(window, &frame_buffer_width, &frame_buffer_height);
    glfwGetWindowSize(window, &window_width, &window_height);
}