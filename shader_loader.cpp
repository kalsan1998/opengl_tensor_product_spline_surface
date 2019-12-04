#include "shader_loader.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::string readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint LoadShader(const char *vertex_path, const char *fragment_path)
{
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders

    std::string vertex_shader_string = readFile(vertex_path);
    std::string fragment_shader_string = readFile(fragment_path);
    const char *vertex_shader_source = vertex_shader_string.c_str();
    const char *fragment_shader_source = fragment_shader_string.c_str();

    GLint result = GL_FALSE;
    int log_length;

    // Compile vertex shader

    std::cout << "Compiling vertex shader." << std::endl;
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    // Check vertex shader

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<GLchar> vertex_shader_error((log_length > 1) ? log_length : 1);
    glGetShaderInfoLog(vertex_shader, log_length, NULL, &vertex_shader_error[0]);
    std::cout << &vertex_shader_error[0] << std::endl;

    // Compile fragment shader

    std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    // Check fragment shader

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<GLchar> fragment_shader_error((log_length > 1) ? log_length : 1);
    glGetShaderInfoLog(fragment_shader, log_length, NULL, &fragment_shader_error[0]);
    std::cout << &fragment_shader_error[0] << std::endl;

    std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<char> programError((log_length > 1) ? log_length : 1);
    glGetProgramInfoLog(program, log_length, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}