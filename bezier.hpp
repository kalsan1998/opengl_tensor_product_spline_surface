#ifndef BEZIER_H
#define BEZIER_H
#include <GL/glew.h>
#include <GL/glut.h>

class BezierDrawer
{
public:
    BezierDrawer();
    void SetControlPoints(float *pts, int number_of_pts);
    void DrawControlPolygon();

private:
    float BasisFn(float u, int ctrl_pt_idx);
    int n;
    float *ctrl_pts;

    GLuint control_pts_vao;
    GLuint control_pts_vbo;
};
#endif