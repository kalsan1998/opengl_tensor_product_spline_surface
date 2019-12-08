#ifndef BSPLINE_H
#define BSPLIBE_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

class BSplineDrawer
{
public:
    BSplineDrawer();
    void DrawBSplineCurve();
    void DrawKnots();
    void DrawControlPoints();
    void DrawControlPolygon();

private:
    void LoadInterpolatedPoints();
    void LoadControlPoints();
    float BSplineBasisFn(float u, int i, int p);

    int interp;
    int deg; // m - n - 1;
    int n;   // n+1 control points;
    int m;   // m+1 knots;
    std::vector<glm::vec3> control_points;
    std::vector<float> knots;

    GLuint curve_points_vao;
    GLuint curve_points_vbo;

    GLuint knot_points_vao;
    GLuint knot_points_vbo;

    GLuint control_points_vao;
    GLuint control_points_vbo;
};

#endif