#include "sphere.hpp"

#include <glm/glm.hpp>
#include <iostream>

SphereDrawer::SphereDrawer() : radius(0.0), rings(0), sectors(0)
{
    glGenVertexArrays(1, &sphere_vao);
    glBindVertexArray(sphere_vao);
    glGenBuffers(1, &sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void SphereDrawer::SetAttributes(float radi, int r, int s)
{
    radius = radi;
    rings = r;
    sectors = s;
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
    glBindVertexArray(sphere_vao);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
}

void SphereDrawer::DrawSphere()
{
    glBindVertexArray(sphere_vao);
    glDrawArrays(GL_TRIANGLES, 0, rings * sectors);
}