#ifndef BSPLINE_SURFACE_H
#define BSPLINE_SURFACE_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

enum SurfaceDrawMode
{
    SURFACE_FILL,
    SURFACE_MESH,
    SURFACE_POINTS,
};

typedef std::vector<std::vector<glm::vec3>> Grid;

class BSplineSurface
{
public:
    BSplineSurface(GLuint program);
    ~BSplineSurface() = default;
    void Draw();

private:
    void DrawBSplineSurface();
    void DrawKnots();
    void DrawControlPoints();
    void DrawControlNet();

    void LoadInterpolatedPoints();
    void LoadKnots();
    void LoadControlPoints();

    float BSplineBasisFn(float u, int i, int p, const std::vector<float> &knots);
    void UpdateKnots();

    GLuint program;
    GLuint color_location;

    int interp;

    GLuint control_points_vao;
    GLuint control_points_vbo;
    GLuint control_element_vbo;

    GLuint interpolated_points_vao;
    GLuint interpolated_points_vbo;
    GLuint interpolated_element_vbo;

    GLuint knot_vao;
    GLuint knot_vbo;

    // For a bicubic spline we must satisfy:
    // h - m - 1 = k - n - 1 = 3
    const int kDeg = 3;

    int m; // m + 1 rows of control points;
    int n; // n + 1 columns of control points;
    Grid control_points;

    int h;                // h + 1 knots in the u direction;
    std::vector<float> U; // knots in the u direction;

    int k;                // k + 1 knots in the v direction;
    std::vector<float> V; // knots in the v direction;

    bool draw_control_net;

    SurfaceDrawMode surface_draw_mode;
};

#endif