#include "bspline_surface.hpp"

#include <iostream>

BSplineSurfaceDrawer::BSplineSurfaceDrawer() : interp(10)
{

    glGenVertexArrays(1, &interpolated_points_vao);
    glBindVertexArray(interpolated_points_vao);
    glGenBuffers(1, &interpolated_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, interpolated_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &interpolated_element_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, interpolated_element_vbo);

    glGenVertexArrays(1, &control_points_vao);
    glBindVertexArray(control_points_vao);
    glGenBuffers(1, &control_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, control_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &control_element_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, control_element_vbo);

    control_points = {
        {
            glm::vec3(-1.0f, -0.3f, 0.0f),
            glm::vec3(-0.25f, 0.25f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.25f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.2f, 0.0f),
        },
        {
            glm::vec3(-1.0f, -0.5f, 0.25f),
            glm::vec3(-0.25f, -0.5f, 0.25f),
            glm::vec3(0.1f, 0.1f, 0.25f),
            glm::vec3(0.25f, 0.3f, 0.25f),
            glm::vec3(1.0f, 0.1f, 0.25f),
        },
        {
            glm::vec3(-1.0f, -0.2f, 0.5f),
            glm::vec3(-0.25f, -0.75f, 0.5f),
            glm::vec3(-0.1f, 0.3f, 0.5f),
            glm::vec3(0.25f, 0.4f, 0.5f),
            glm::vec3(1.0f, 0.1f, 0.5f),
        },
        {
            glm::vec3(-1.0f, -0.2f, 0.75f),
            glm::vec3(-0.25f, -0.75f, 0.75f),
            glm::vec3(-0.2f, 0.0f, 0.75f),
            glm::vec3(0.25f, 0.4f, 0.75f),
            glm::vec3(1.0f, 0.1f, 0.75f),
        },
        {
            glm::vec3(-1.0f, -0.2f, 1.0f),
            glm::vec3(-0.25f, -0.75f, 1.0f),
            glm::vec3(-0.2f, 0.0f, 1.0f),
            glm::vec3(0.25f, 0.4f, 1.0f),
            glm::vec3(1.0f, 0.1f, 1.0f),
        },
    };
    U = {0.0f, 0.0f, 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.0f, 1.0f};
    V = {0.0f, 0.0f, 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.0f, 1.0f};
    m = control_points.size() - 1;
    n = control_points[0].size() - 1;
    h = U.size() - 1;
    k = V.size() - 1;

    LoadInterpolatedPoints();
    LoadControlPoints();
}

void BSplineSurfaceDrawer::DrawBSplineSurface()
{
    glBindVertexArray(interpolated_points_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, interpolated_element_vbo);
    glDrawElements(GL_TRIANGLES, (interp - 1) * (interp - 1) * 6, GL_UNSIGNED_SHORT, 0);
}

void BSplineSurfaceDrawer::DrawControlPoints()
{
    glBindVertexArray(control_points_vao);
    glDrawArrays(GL_POINTS, 0, (n + 1) * (m + 1));
}

void BSplineSurfaceDrawer::DrawControlNet()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(control_points_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, control_element_vbo);
    glDrawElements(GL_QUADS, m * n * 4, GL_UNSIGNED_SHORT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// TODO: Optimize accoridng to https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/surface/bspline-de-boor.html
void BSplineSurfaceDrawer::LoadInterpolatedPoints()
{
    int p = h - m - 1;
    int q = k - n - 1;
    std::vector<glm::vec3> interp_vertices(interp * interp * 2);
    for (int u = 0; u < interp; ++u)
    {
        float norm_u = U[p] + ((float)u / (float)interp) * (U[m + 1] - U[p]);
        for (int v = 0; v < interp; ++v)
        {
            float norm_v = V[q] + ((float)v / (float)interp) * (V[n + 1] - V[q]);
            glm::vec3 c_u(0.0f);
            glm::vec3 du(0.0f);
            glm::vec3 dv(0.0f);
            for (int i = 0; i < m + 1; ++i)
            {
                float N_u = BSplineBasisFn(norm_u, i, p, U);
                for (int j = 0; j < n + 1; ++j)
                {
                    c_u += N_u * BSplineBasisFn(norm_v, j, q, V) * control_points[i][j];
                    if (i != m)
                        du += BSplineBasisFn(norm_u, i + 1, p - 1, U) * (p / (U[i + p + 1] - U[i + 1])) * (control_points[i + 1][j] - control_points[i][j]);
                    if (j != n)
                        dv += BSplineBasisFn(norm_v, j + 1, q - 1, V) * (q / (V[j + q + 1] - V[j + 1])) * (control_points[i][j + 1] - control_points[i][j]);
                }
            }
            glm::vec3 normal = glm::cross(du, dv);
            interp_vertices[(u * interp * 2) + v * 2] = c_u;
            interp_vertices[(u * interp * 2) + v * 2 + 1] = normal;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, interpolated_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, interp_vertices.size() * 3 * sizeof(float), &interp_vertices[0], GL_STATIC_DRAW);

    std::vector<GLushort> indices((interp - 1) * (interp - 1) * 6); // Each cell hs 2 triangles == 6 vertices;
    for (int i = 0; i < interp - 1; ++i)
    {
        for (int j = 0; j < interp - 1; ++j)
        {
            int base = i * (interp - 1) * 6 + (j * 6);
            indices[base] = i * interp + j;
            indices[base + 1] = i * interp + j + 1;
            indices[base + 2] = (i + 1) * interp + j;
            indices[base + 3] = indices[base + 1];
            indices[base + 4] = indices[base + 2];
            indices[base + 5] = indices[base + 2] + 1;
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, interpolated_element_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void BSplineSurfaceDrawer::LoadControlPoints()
{
    std::vector<glm::vec3> points;
    int i = 0;
    for (auto &u : control_points)
    {
        for (auto v : u)
        {
            points.push_back(v);
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, control_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * 3 * sizeof(float), &points[0], GL_STATIC_DRAW);

    std::vector<GLushort> indices(m * n * 4);
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            int base = 4 * ((i * n) + j);
            indices[base] = i * (n + 1) + j;
            indices[base + 1] = i * (n + 1) + j + 1;
            indices[base + 2] = (i + 1) * (n + 1) + j + 1;
            indices[base + 3] = (i + 1) * (n + 1) + j;
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, control_element_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

// TODO: DP
float BSplineSurfaceDrawer::BSplineBasisFn(float u, int i, int p, const std::vector<float> &knots)
{
    if (p == 0)
    {
        if (knots[i] <= u && u < knots[i + 1])
            return 1;
        return 0;
    }
    float n_1 = BSplineBasisFn(u, i, p - 1, knots);
    float term_1 = n_1 == 0 ? 0 : ((u - knots[i]) / (knots[i + p] - knots[i])) * n_1;
    float n_2 = BSplineBasisFn(u, i + 1, p - 1, knots);
    float term_2 = n_2 == 0 ? 0 : ((knots[i + p + 1] - u) / (knots[i + p + 1] - knots[i + 1])) * n_2;
    return term_1 + term_2;
}
