#include "bezier.hpp"

#include <boost/math/special_functions/factorials.hpp>
#include <cmath>
#include <iostream>

using boost::math::factorial;
using std::pow;

BezierDrawer::BezierDrawer() : n(0)
{
    glGenVertexArrays(1, &control_pts_vao);
    glGenBuffers(1, &control_pts_vbo);
}

void BezierDrawer::SetControlPoints(float *pts, int n_pts)
{
    ctrl_pts = pts;
    n = n_pts - 1;
}

void BezierDrawer::DrawControlPolygon()
{
    float control[(n + 1) * 3];
    for (int i = 0; i < (n + 1) * 3; ++i)
    {
        control[i] = ctrl_pts[i];
    }
    glBindVertexArray(control_pts_vao);
    glBindBuffer(GL_ARRAY_BUFFER, control_pts_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(control), control, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, n + 1);
    // glDisableVertexAttribArray(0);
}

float BezierDrawer::BasisFn(float u, int i)
{
    return (factorial<float>(n) / (factorial<float>(n - i) * factorial<float>(i))) * pow(u, i) * pow(1 - u, n - i);
}