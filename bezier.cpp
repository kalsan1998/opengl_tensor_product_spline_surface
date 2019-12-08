#include "bezier.hpp"

#include <boost/math/special_functions/factorials.hpp>
#include <cmath>
#include <iostream>

using boost::math::factorial;
using std::pow;

BezierDrawer::BezierDrawer() : n(3), interp(30)
{
    glGenVertexArrays(1, &ctrl_pts_vao);
    glBindVertexArray(ctrl_pts_vao);
    glGenBuffers(1, &ctrl_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, ctrl_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &interp_pts_vao);
    glBindVertexArray(interp_pts_vao);
    glGenBuffers(1, &interp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, interp_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    control_points = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(-0.25f, 0.75f, 0.0f),
        glm::vec3(0.5f, -0.25f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f)};
    LoadControlPoints();
    LoadInterpolatedPoints();
}

void BezierDrawer::DrawControlPoints()
{
    glBindVertexArray(ctrl_pts_vao);
    glPointSize(6.0f);
    glDrawArrays(GL_POINTS, 0, n + 1);
}

void BezierDrawer::DrawControlPolygon()
{
    glBindVertexArray(ctrl_pts_vao);
    glLineWidth(1.0f);
    glDrawArrays(GL_LINE_STRIP, 0, n + 1);
}

void BezierDrawer::DrawBezierCurve()
{
    glBindVertexArray(interp_pts_vao);
    glLineWidth(3.0f);
    glDrawArrays(GL_LINE_STRIP, 0, interp);
}

float BezierDrawer::BasisFn(float u, int i)
{
    return (factorial<float>(n) / (factorial<float>(n - i) * factorial<float>(i))) * pow(u, i) * pow(1 - u, n - i);
}

void BezierDrawer::LoadControlPoints()
{
    glBindBuffer(GL_ARRAY_BUFFER, ctrl_vbo);
    glBufferData(GL_ARRAY_BUFFER, control_points.size() * 3 * sizeof(float), &control_points[0], GL_STATIC_DRAW);
}

void BezierDrawer::LoadInterpolatedPoints()
{
    std::vector<glm::vec3> interp_vertices(interp);
    for (int u = 0; u < interp; ++u)
    {
        float norm_u = (float)u / (float)interp;
        glm::vec3 c_u(0.0f);
        for (int i = 0; i < n + 1; ++i)
        {
            c_u += BasisFn(norm_u, i) * control_points[i];
        }
        interp_vertices[u] = c_u;
    }

    glBindBuffer(GL_ARRAY_BUFFER, interp_vbo);
    glBufferData(GL_ARRAY_BUFFER, interp_vertices.size() * 3 * sizeof(float), &interp_vertices[0], GL_STATIC_DRAW);
}