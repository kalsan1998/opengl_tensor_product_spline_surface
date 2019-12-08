#include "bspline_surface.hpp"

#include <iostream>

BSplineSurfaceDrawer::BSplineSurfaceDrawer() : interp(10)
{

    glGenVertexArrays(1, &interpolated_points_vao);
    glBindVertexArray(interpolated_points_vao);
    glGenBuffers(1, &interpolated_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, interpolated_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    control_points = {
        {
            glm::vec3(-1.0f, -1.0f, 0.0f),
            glm::vec3(-0.25f, 0.75f, 0.0f),
            glm::vec3(0.25f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.2f, 0.0f),
        },
        {
            glm::vec3(-1.0f, -0.5f, 0.25f),
            glm::vec3(-0.25f, -0.5f, 0.25f),
            glm::vec3(0.25f, 0.3f, 0.25f),
            glm::vec3(1.0f, 0.1f, 0.25f),
        },
        {
            glm::vec3(-1.0f, -0.2f, 0.5f),
            glm::vec3(-0.25f, -0.75f, 0.5f),
            glm::vec3(0.25f, 0.4f, 0.5f),
            glm::vec3(1.0f, 0.1f, 0.5f),
        },
        {
            glm::vec3(-1.0f, -0.2f, 0.75f),
            glm::vec3(-0.25f, -0.75f, 0.75f),
            glm::vec3(0.25f, 0.4f, 0.75f),
            glm::vec3(1.0f, 0.1f, 0.75f),
        },
    };
    U = {0.0f, 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.0f};
    V = {0.0f, 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.0f};
    m = control_points.size() - 1;
    n = control_points[0].size() - 1;
    h = U.size() - 1;
    k = V.size() - 1;

    LoadInterpolatedPoints();
}

void BSplineSurfaceDrawer::DrawBSplineSurface()
{
    glBindVertexArray(interpolated_points_vao);
    glDrawArrays(GL_QUAD_STRIP, 0, interp * interp);
}

void BSplineSurfaceDrawer::LoadInterpolatedPoints()
{
    int p = h - m - 1;
    int q = k - n - 1;
    std::vector<glm::vec3> interp_vertices(interp * interp);
    for (int u = 0; u < interp; ++u)
    {
        float norm_u = U[p] + ((float)u / (float)interp) * (U[m + 1] - U[p]);
        for (int v = 0; v < interp; ++v)
        {
            float norm_v = V[q] + ((float)v / (float)interp) * (V[n + 1] - V[q]);
            glm::vec3 c_u(0.0f);
            for (int i = 0; i < m + 1; ++i)
            {
                for (int j = 0; j < n + 1; ++j)
                {
                    c_u += BSplineBasisFn(norm_u, i, p, U) * BSplineBasisFn(norm_v, j, q, V) * control_points[i][j];
                }
            }
            interp_vertices[(u * interp) + v] = c_u;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, interpolated_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, interp_vertices.size() * 3 * sizeof(float), &interp_vertices[0], GL_STATIC_DRAW);
}

// TODO: DP
float BSplineSurfaceDrawer::BSplineBasisFn(float u, int i, int p, std::vector<float> &knots)
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