#include "sphere.hpp"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

SphereDrawer::SphereDrawer() : radius(1.0f), rings(12), sectors(36), draw_mode(GL_POINTS)
{
    glGenVertexArrays(1, &sphere_vao);
    glBindVertexArray(sphere_vao);
    glGenBuffers(1, &sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &element_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo);

    LoadVertices();
}

void SphereDrawer::LoadVertices()
{
    float R = 1.0f / (float)(rings - 1);
    float S = 1.0f / (float)(sectors - 1);
    float v[rings * sectors * 6];
    for (int r = 0; r < rings; ++r)
    {
        for (int s = 0; s < sectors; ++s)
        {
            float y = glm::sin(-M_PI_2 + M_PI * r * R);
            float x = glm::cos(2 * M_PI * s * S) * glm::sin(M_PI * r * R);
            float z = glm::sin(2 * M_PI * s * S) * glm::sin(M_PI * r * R);

            int base = (r * sectors * 6) + (s * 6);
            v[base] = x * radius;
            v[base + 1] = y * radius;
            v[base + 2] = z * radius;
            v[base + 3] = x;
            v[base + 4] = y;
            v[base + 5] = z;
        }
    }
    GLushort indices[rings * sectors * 4];
    for (int r = 0; r < rings; ++r)
    {
        for (int s = 0; s < sectors; ++s)
        {
            int base = (r * sectors * 4) + (s * 4);
            indices[base] = r * sectors + s;
            indices[base + 1] = r * sectors + (s + 1);
            indices[base + 2] = (r + 1) * sectors + (s + 1);
            indices[base + 3] = (r + 1) * sectors + s;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void SphereDrawer::DrawSphere()
{
    glBindVertexArray(sphere_vao);
    glPointSize(1.0f);
    glLineWidth(1.0f);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo);
    glDrawElements(draw_mode, rings * sectors * 4, GL_UNSIGNED_SHORT, 0);
}

void SphereDrawer::ProcessKeys(int key, int action)
{
    if (key == GLFW_KEY_P)
    {
        draw_mode = GL_POINTS;
    }
    if (key == GLFW_KEY_Q)
    {
        draw_mode = GL_QUADS;
    }
    if (key == GLFW_KEY_L)
    {
        draw_mode = GL_LINE_STRIP;
    }
    if (key == GLFW_KEY_W)
    {
        draw_mode = GL_POINTS;
    }
    if (key == GLFW_KEY_EQUAL)
    {
        sectors++;
        LoadVertices();
    }
    if (key == GLFW_KEY_MINUS)
    {
        sectors = std::max(4, sectors - 1);
        LoadVertices();
    }
    if (key == GLFW_KEY_0)
    {
        rings++;
        LoadVertices();
    }
    if (key == GLFW_KEY_9)
    {
        rings = std::max(3, rings - 1);
        LoadVertices();
    }
}
