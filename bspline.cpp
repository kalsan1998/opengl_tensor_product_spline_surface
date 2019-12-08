#include "bspline.hpp"

#include <iostream>

BSplineDrawer::BSplineDrawer() : interp(60), n(5), m(8)
{
    glGenVertexArrays(1, &curve_points_vao);
    glBindVertexArray(curve_points_vao);
    glGenBuffers(1, &curve_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, curve_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    control_points = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(-0.25f, 0.75f, 0.0f),
        glm::vec3(0.5f, -0.25f, 0.0f),
        glm::vec3(0.8f, 0.0f, 0.0f),
        glm::vec3(0.7f, 0.6f, 0.0f),
        glm::vec3(0.3f, 0.2f, 0.0f),
    };
    deg = m - n - 1;
    knots = {0.0f, 0.0f, 0.2f, 0.4f, 0.5f, 0.6f, 0.8f, 1.0f, 1.0f};
    LoadInterpolatedPoints();
}

void BSplineDrawer::DrawBSplineCurve()
{
    glBindVertexArray(curve_points_vao);
    glLineWidth(3.0);
    glDrawArrays(GL_LINE_STRIP, 0, interp);
}

void BSplineDrawer::LoadInterpolatedPoints()
{
    std::vector<glm::vec3> interp_vertices(interp);
    for (int u = 0; u < interp; ++u)
    {
        float norm_u = knots[deg] + ((float)u / (float)interp) * (knots[m - deg] - knots[deg]);
        glm::vec3 c_u(0.0f);
        for (int i = 0; i < n + 1; ++i)
        {
            c_u += BSplineBasisFn(norm_u, i, deg) * control_points[i];
        }
        interp_vertices[u] = c_u;
    }

    glBindVertexArray(curve_points_vao);
    glBindBuffer(GL_ARRAY_BUFFER, curve_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, interp_vertices.size() * 3 * sizeof(float), &interp_vertices[0], GL_STATIC_DRAW);
}
// TODO: DP
float BSplineDrawer::BSplineBasisFn(float u, int i, int p)
{
    if (p == 0)
    {
        if (knots[i] <= u && u < knots[i + 1])
            return 1;
        return 0;
    }
    float n_1 = BSplineBasisFn(u, i, p - 1);
    float term_1 = n_1 == 0 ? 0 : ((u - knots[i]) / (knots[i + p] - knots[i])) * n_1;
    float n_2 = BSplineBasisFn(u, i + 1, p - 1);
    float term_2 = n_2 == 0 ? 0 : ((knots[i + p + 1] - u) / (knots[i + p + 1] - knots[i + 1])) * n_2;
    return term_1 + term_2;
}