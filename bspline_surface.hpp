#ifndef BSPLINE_SURFACE_H
#define BSPLINE_SURFACE_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

typedef std::vector<std::vector<glm::vec3>> Grid;

class BSplineSurfaceDrawer
{
public:
    BSplineSurfaceDrawer();
    void Draw();
    void ProcessKeys(int key, int action);

private:
    void DrawBSplineSurface();
    void DrawControlPoints();
    void DrawControlNet();

    void LoadInterpolatedPoints();
    void LoadControlPoints();
    float BSplineBasisFn(float u, int i, int p, const std::vector<float> &knots);

    int interp;

    GLuint color_location;

    GLuint control_points_vao;
    GLuint control_points_vbo;
    GLuint control_element_vbo;

    GLuint interpolated_points_vao;
    GLuint interpolated_points_vbo;
    GLuint interpolated_element_vbo;

    int m; // m + 1 rows of control points;
    int n; // n + 1 columns of control points;
    Grid control_points;

    int h;                // h + 1 knots in the u direction;
    std::vector<float> U; // knots in the u direction;

    int k;                // k + 1 knots in the v direction;
    std::vector<float> V; // knots in the v direction;
};

#endif