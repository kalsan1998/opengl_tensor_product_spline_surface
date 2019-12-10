#ifndef BEZIER_H
#define BEZIER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

#include <vector>

class BezierDrawer
{
public:
    BezierDrawer();
    void DrawControlPoints();
    void DrawControlPolygon();
    void DrawBezierCurve();

private:
    void LoadInterpolatedPoints();
    void LoadControlPoints();
    float BasisFn(float u, int ctrl_pt_idx);
    int n;
    int interp;
    std::vector<glm::vec3> control_points;

    GLuint ctrl_pts_vao;
    GLuint interp_pts_vao;

    GLuint ctrl_vbo;
    GLuint interp_vbo;
};
#endif