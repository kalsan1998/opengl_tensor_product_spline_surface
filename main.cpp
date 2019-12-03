#include <iostream>

#include "load_shader.hpp"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

void Draw()
{
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

GLuint CreateShader(GLenum type, const GLchar *src)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(shader, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    return shader;
}

// Read the Fragment Shader code from the
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
    window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
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

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, kVertexShaderSource);
    GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, kFragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    char *infoLog;
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);
}