#include "bspline.hpp"

BSplineDrawer::BSplineDrawer() : interp(30), n(0), m(0)
{
    glGenVertexArrays(1, &curve_points_vao);
    glBindVertexArray(curve_points_vao);
    glGenBuffers(1, &curve_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, curve_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void BSplineDrawer::DrawBSplineCurve()
{
    glBindVertexArray(curve_points_vao);
    glLineWidth(3.0);
    glDrawArrays(GL_LINE_STRIP, 0, interp);
}

void BSplineDrawer::SetControlPoints(std::vector<glm::vec3> control_pts)
{
    control_points = control_pts;
    n = control_pts.size() - 1;
    LoadInterpolatedPoints();
}

void BSplineDrawer::SetKnots(std::vector<float> k)
{
    knots = k;
    m = k.size() - 1;
    LoadInterpolatedPoints();
}

void BSplineDrawer::LoadInterpolatedPoints()
{
    float interp_vertices[interp * 3];
    for (int u = 0; u < interp; ++u)
    {
        float norm_u = (float)u / (float)interp;
        glm::vec3 c_u(0.0f);
        for (int i = 0; i < n + 1; ++i)
        {
            c_u += BSplineBasisFn(norm_u, i, m + n - 1) * control_points[i];
        }
        interp_vertices[u * 3] = c_u.x;
        interp_vertices[(u * 3) + 1] = c_u.y;
        interp_vertices[(u * 3) + 2] = c_u.z;
    }

    glBindVertexArray(curve_points_vao);
    glBindBuffer(GL_ARRAY_BUFFER, curve_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(interp_vertices), interp_vertices, GL_STATIC_DRAW);
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
    float term_1 = ((u - knots[i]) / (knots[i + p] - knots[i])) * BSplineBasisFn(u, i, p - 1);
    float term_2 = ((knots[i + p + 1] - u) / (knots[u + p + 1] - knots[i + 1])) * BSplineBasisFn(u, i + 1, p - 1);
    return term_1 + term_2;
}