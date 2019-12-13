#ifndef BSPLINE_SURFACE_H
#define BSPLINE_SURFACE_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "drawer.hpp"

enum SurfaceDrawMode
{
    SURFACE_FILL,
    SURFACE_MESH,
    SURFACE_POINTS,
};

typedef std::vector<std::vector<glm::vec3>> Grid;

class BSplineSurfaceDrawer : public Drawer
{
public:
    BSplineSurfaceDrawer(GLuint program);
    ~BSplineSurfaceDrawer() = default;
    void Draw() override;
    void ProcessKeys(int key, int action) override;

private:
    void DrawBSplineSurface();
    void DrawKnots();
    void DrawControlPoints();
    void DrawControlNet();

    void LoadInterpolatedPoints();
    void LoadKnots();
    void LoadControlPoints();
    float BSplineBasisFn(float u, int i, int p, const std::vector<float> &knots);

    int interp;

    GLuint control_points_vao;
    GLuint control_points_vbo;
    GLuint control_element_vbo;

    GLuint interpolated_points_vao;
    GLuint interpolated_points_vbo;
    GLuint interpolated_element_vbo;

    GLuint knot_vao;
    GLuint knot_vbo;

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