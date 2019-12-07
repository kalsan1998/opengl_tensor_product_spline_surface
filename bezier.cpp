#include "bezier.hpp"

#include <boost/math/special_functions/factorials.hpp>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

using boost::math::factorial;
using std::pow;

BezierDrawer::BezierDrawer() : n(0), interp(30)
{
    glGenVertexArrays(1, &control_pts_vao);
    glGenBuffers(1, &control_pts_vbo);
    glGenVertexArrays(1, &interp_pts_vao);
    glGenBuffers(1, &interp_pts_vbo);
}

void BezierDrawer::SetControlPoints(float *pts, int n_pts)
{
    ctrl_pts = pts;
    n = n_pts - 1;

    float vertices[n_pts * 3];
    for (int i = 0; i < n_pts * 3; ++i)
    {
        vertices[i] = ctrl_pts[i];
    }
    glBindVertexArray(control_pts_vao);
    glBindBuffer(GL_ARRAY_BUFFER, control_pts_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    float interp_vertices[interp * 3];
    for (int u = 0; u < interp; ++u)
    {
        float norm_u = (float)u / (float)interp;
        glm::vec3 c_u(0.0f);
        for (int i = 0; i < n_pts; ++i)
        {
            c_u += BasisFn(norm_u, i) * glm::vec3(ctrl_pts[i * 3], ctrl_pts[(i * 3) + 1], ctrl_pts[(i * 3) + 2]);
        }
        interp_vertices[u * 3] = c_u.x;
        interp_vertices[(u * 3) + 1] = c_u.y;
        interp_vertices[(u * 3) + 2] = c_u.z;
    }

    glBindVertexArray(interp_pts_vao);
    glBindBuffer(GL_ARRAY_BUFFER, interp_pts_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(interp_vertices), interp_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void BezierDrawer::DrawControlPolygon()
{
    // TODO: draw control points separately
    glBindVertexArray(control_pts_vao);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, n + 1);
    glDisableVertexAttribArray(0);
}

void BezierDrawer::DrawBezierCurve()
{
    glBindVertexArray(interp_pts_vao);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, interp);
    glDisableVertexAttribArray(0);
}

float BezierDrawer::BasisFn(float u, int i)
{
    return (factorial<float>(n) / (factorial<float>(n - i) * factorial<float>(i))) * pow(u, i) * pow(1 - u, n - i);
}