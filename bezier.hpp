#ifndef BEZIER_H
#define BEZIER_H
#include <GL/glew.h>
#include <GL/glut.h>

class BezierDrawer
{
public:
    BezierDrawer();
    void SetControlPoints(float *pts, int number_of_pts);
    void DrawControlPoints();
    void DrawControlPolygon();
    void DrawBezierCurve();

private:
    void LoadInterpolatedPoints();
    void LoadControlPoints();
    float BasisFn(float u, int ctrl_pt_idx);
    int n;
    int interp;
    float *ctrl_pts;

    GLuint ctrl_pts_vao;
    GLuint interp_pts_vao;

    GLuint ctrl_vbo;
    GLuint interp_vbo;
};
#endif