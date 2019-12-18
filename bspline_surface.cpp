#include "bspline_surface.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui/imgui.h"

const float EPSILON = 0.0001;
BSplineSurfaceDrawer::BSplineSurfaceDrawer(GLuint program) : interp(25), draw_control_net(true), surface_draw_mode(SURFACE_MESH)
{
    this->program = program;
    color_location = glGetUniformLocation(program, "uni_color");

    glGenVertexArrays(1, &interpolated_points_vao);
    glBindVertexArray(interpolated_points_vao);
    glGenBuffers(1, &interpolated_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, interpolated_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &interpolated_element_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, interpolated_element_vbo);

    glGenVertexArrays(1, &control_points_vao);
    glBindVertexArray(control_points_vao);
    glGenBuffers(1, &control_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, control_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &control_element_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, control_element_vbo);

    glGenVertexArrays(1, &knot_vao);
    glBindVertexArray(knot_vao);
    glGenBuffers(1, &knot_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, knot_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    control_points = {
        {
            glm::vec3(-1.0f, -0.3f, 0.0f),
            glm::vec3(-0.25f, 0.25f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.25f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.2f, 0.0f),
        },
        {
            glm::vec3(-1.0f, -0.5f, 0.25f),
            glm::vec3(-0.25f, -0.5f, 0.25f),
            glm::vec3(0.1f, 0.1f, 0.25f),
            glm::vec3(0.25f, 0.3f, 0.25f),
            glm::vec3(1.0f, 0.1f, 0.25f),
        },
        {
            glm::vec3(-1.0f, -0.2f, 0.5f),
            glm::vec3(-0.25f, -0.75f, 0.5f),
            glm::vec3(-0.1f, 0.3f, 0.5f),
            glm::vec3(0.25f, 0.4f, 0.5f),
            glm::vec3(1.0f, 0.1f, 0.5f),
        },
        {
            glm::vec3(-1.0f, -0.2f, 0.75f),
            glm::vec3(-0.25f, -0.75f, 0.75f),
            glm::vec3(-0.2f, 0.0f, 0.75f),
            glm::vec3(0.25f, 0.4f, 0.75f),
            glm::vec3(1.0f, 0.1f, 0.75f),
        },
        {
            glm::vec3(-1.0f, -0.2f, 1.0f),
            glm::vec3(-0.25f, -0.75f, 1.0f),
            glm::vec3(-0.2f, 0.0f, 1.0f),
            glm::vec3(0.25f, 0.4f, 1.0f),
            glm::vec3(1.0f, 0.1f, 1.0f),
        },
    };
    U = {0.0f, 0.0f, 0.0f, 0.25f, 0.75f, 1.0f, 1.0f, 1.0f};
    V = {0.0f, 0.0f, 0.0f, 0.25f, 0.75f, 1.0f, 1.0f, 1.0f};
    m = control_points.size() - 1;
    n = control_points[0].size() - 1;
    h = U.size() - 1;
    k = V.size() - 1;
    U[h] += EPSILON;
    V[k] += EPSILON;

    LoadInterpolatedPoints();
    LoadKnots();
    LoadControlPoints();
}

void BSplineSurfaceDrawer::Draw()
{
    DrawBSplineSurface();
    DrawKnots();

    if (draw_control_net)
    {
        DrawControlPoints();
        DrawControlNet();
    }
}

void BSplineSurfaceDrawer::DrawBSplineSurface()
{
    float surface_color[3] = {0.7f, 0.7f, 0.0f};
    glUniform3fv(color_location, 1, surface_color);
    glBindVertexArray(interpolated_points_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, interpolated_element_vbo);
    if (surface_draw_mode == SURFACE_FILL)
    {
        glEnableVertexAttribArray(1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (surface_draw_mode == SURFACE_MESH)
    {
        glDisableVertexAttribArray(1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (surface_draw_mode == SURFACE_POINTS)
    {
        glDisableVertexAttribArray(1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
    glDrawElements(GL_TRIANGLES, (interp - 1) * (interp - 1) * 6, GL_UNSIGNED_SHORT, 0);
}

void BSplineSurfaceDrawer::DrawControlPoints()
{
    float point_color[3] = {0.9f, 0.5f, 0.0f};
    glUniform3fv(color_location, 1, point_color);
    glPointSize(5.0f);
    glBindVertexArray(control_points_vao);
    glDrawArrays(GL_POINTS, 0, (n + 1) * (m + 1));
    glPointSize(1.0f);
}

void BSplineSurfaceDrawer::DrawControlNet()
{
    float net_color[3] = {1.0f, 0.0f, 0.0f};
    glUniform3fv(color_location, 1, net_color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(control_points_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, control_element_vbo);
    glDrawElements(GL_QUADS, m * n * 4, GL_UNSIGNED_SHORT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BSplineSurfaceDrawer::DrawKnots()
{
    float point_color[3] = {0.0f, 0.0f, 1.0f};
    glUniform3fv(color_location, 1, point_color);
    glPointSize(5.0f);
    glBindVertexArray(knot_vao);
    glDrawArrays(GL_POINTS, 0, (2 * m + 3 - h) * (2 * n + 3 - k));
    glPointSize(1.0f);
}

// TODO: Optimize accoridng to https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/surface/bspline-de-boor.html
void BSplineSurfaceDrawer::LoadInterpolatedPoints()
{
    int p = h - m - 1;
    int q = k - n - 1;
    std::vector<glm::vec3> interp_vertices(interp * interp * 2);
    for (int u = 0; u < interp; ++u)
    {
        float norm_u = U[p] + ((float)u / (float)(interp - 1)) * (U[m + 1] - U[p]);
        for (int v = 0; v < interp; ++v)
        {
            float norm_v = V[q] + ((float)v / (float)(interp - 1)) * (V[n + 1] - V[q]);
            glm::vec3 c_u(0.0f);
            glm::vec3 du(0.0f);
            glm::vec3 dv(0.0f);
            for (int i = 0; i < m + 1; ++i)
            {
                float N_u = BSplineBasisFn(norm_u, i, p, U);
                for (int j = 0; j < n + 1; ++j)
                {
                    c_u += N_u * BSplineBasisFn(norm_v, j, q, V) * control_points[i][j];
                    if (i != m)
                        du += BSplineBasisFn(norm_u, i + 1, p - 1, U) * (p / (U[i + p + 1] - U[i + 1])) * (control_points[i + 1][j] - control_points[i][j]);
                    if (j != n)
                        dv += BSplineBasisFn(norm_v, j + 1, q - 1, V) * (q / (V[j + q + 1] - V[j + 1])) * (control_points[i][j + 1] - control_points[i][j]);
                }
            }
            glm::vec3 normal = glm::cross(du, dv);
            interp_vertices[(u * interp * 2) + v * 2] = c_u;
            interp_vertices[(u * interp * 2) + v * 2 + 1] = normal;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, interpolated_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, interp_vertices.size() * 3 * sizeof(float), &interp_vertices[0], GL_STATIC_DRAW);

    std::vector<GLushort> indices((interp - 1) * (interp - 1) * 6); // Each cell hs 2 triangles == 6 vertices;
    for (int i = 0; i < interp - 1; ++i)
    {
        for (int j = 0; j < interp - 1; ++j)
        {
            int base = i * (interp - 1) * 6 + (j * 6);
            indices[base] = i * interp + j;
            indices[base + 1] = i * interp + j + 1;
            indices[base + 2] = (i + 1) * interp + j;
            indices[base + 3] = indices[base + 1];
            indices[base + 4] = indices[base + 2];
            indices[base + 5] = indices[base + 2] + 1;
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, interpolated_element_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void BSplineSurfaceDrawer::LoadControlPoints()
{
    std::vector<glm::vec3> points;
    for (auto &u : control_points)
    {
        for (auto v : u)
        {
            points.push_back(v);
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, control_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * 3 * sizeof(float), &points[0], GL_STATIC_DRAW);

    std::vector<GLushort> indices(m * n * 4);
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            int base = 4 * ((i * n) + j);
            indices[base] = i * (n + 1) + j;
            indices[base + 1] = i * (n + 1) + j + 1;
            indices[base + 2] = (i + 1) * (n + 1) + j + 1;
            indices[base + 3] = (i + 1) * (n + 1) + j;
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, control_element_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

// TODO: Optimize accoridng to https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/surface/bspline-de-boor.html
void BSplineSurfaceDrawer::LoadKnots()
{
    int p = h - m - 1;
    int q = k - n - 1;
    std::vector<glm::vec3> knot_points;
    for (int u = p; u <= m + 1; ++u)
    {
        float k_u = U[u];
        for (int v = q; v <= n + 1; ++v)
        {
            float k_v = V[v];
            glm::vec3 c_u(0.0f);
            for (int i = 0; i < m + 1; ++i)
            {
                float N_u = BSplineBasisFn(k_u, i, p, U);
                for (int j = 0; j < n + 1; ++j)
                {
                    c_u += N_u * BSplineBasisFn(k_v, j, q, V) * control_points[i][j];
                }
            }
            knot_points.push_back(c_u);
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, knot_vbo);
    glBufferData(GL_ARRAY_BUFFER, knot_points.size() * 3 * sizeof(float), &knot_points[0], GL_STATIC_DRAW);
}

// TODO: DP
float BSplineSurfaceDrawer::BSplineBasisFn(float u, int i, int p, const std::vector<float> &knots)
{
    if (p <= 0)
    {
        if (knots[i] <= u && u < knots[i + 1])
            return 1;
        return 0;
    }
    float n_1 = BSplineBasisFn(u, i, p - 1, knots);
    float term_1 = n_1 == 0 ? 0 : ((u - knots[i]) / (knots[i + p] - knots[i])) * n_1;
    float n_2 = BSplineBasisFn(u, i + 1, p - 1, knots);
    float term_2 = n_2 == 0 ? 0 : ((knots[i + p + 1] - u) / (knots[i + p + 1] - knots[i + 1])) * n_2;
    return term_1 + term_2;
}

void BSplineSurfaceDrawer::UpdateControlPointCounts(int new_m, int new_n)
{
    if (new_n == n && new_m == m)
        return;
    if (new_n < 0)
        new_n = 0;
    if (new_n > k - 1)
        new_n = k - 1;
    if (new_m < 0)
        new_m = 0;
    if (new_m > h - 1)
        new_m = h - 1;
    if (new_n != n)
    {
        for (auto &v : control_points)
        {
            v.resize(new_n + 1);
            for (int i = n + 1; i < new_n + 1; ++i)
            {
                v[i] = v[n];
            }
        }
        n = new_n;
    }
    if (new_m != m)
    {
        control_points.resize(new_m + 1);
        for (int i = m + 1; i < new_m + 1; ++i)
        {
            control_points[i] = control_points[m];
        }
        m = new_m;
    }
    LoadControlPoints();
    LoadInterpolatedPoints();
    LoadKnots();
}

void BSplineSurfaceDrawer::UpdateKnotCounts(int new_h, int new_k)
{
    if (new_h == h && new_k == k)
        return;
    if (new_h < m + 1)
        new_h = m + 1;
    if (new_k < n + 1)
        new_k = n + 1;
    if (new_h != h)
    {
        U[h] -= EPSILON;
        U.resize(new_h + 1);
        for (int i = h + 1; i < new_h + 1; ++i)
        {
            U[i] = U[h];
        }
        h = new_h;
        U[h] += EPSILON;
    }
    if (new_k != k)
    {
        V[k] -= EPSILON;
        V.resize(new_k + 1);
        for (int i = k + 1; i < new_k + 1; ++i)
        {
            V[i] = V[k];
        }
        k = new_k;
        V[k] += EPSILON;
    }
    LoadKnots();
    LoadInterpolatedPoints();
}

void BSplineSurfaceDrawer::ProcessKeys(int key, int action)
{
    if (action == GLFW_RELEASE)
        return;
    if (key == GLFW_KEY_C)
    {
        draw_control_net = !draw_control_net;
    }
    else if (key == GLFW_KEY_Q)
    {
        surface_draw_mode = SURFACE_FILL;
    }
    else if (key == GLFW_KEY_W)
    {
        surface_draw_mode = SURFACE_MESH;
    }
    else if (key == GLFW_KEY_E)
    {
        surface_draw_mode = SURFACE_POINTS;
    }
    else if (key == GLFW_KEY_MINUS)
    {
        interp = interp == 3 ? interp : interp - 1;
        LoadInterpolatedPoints();
    }
    else if (key == GLFW_KEY_EQUAL)
    {
        interp += 1;
        LoadInterpolatedPoints();
    }
}
void BSplineSurfaceDrawer::GuiLogic()
{
    int ctrl_point_counts[2] = {m + 1, n + 1};
    int degrees[2] = {h - m - 1, k - n - 1};
    int step = 1;
    ImGui::InputScalarN("Control Points (u,v)", ImGuiDataType_S32, ctrl_point_counts, 2, &step, NULL, "%d");
    ImGui::InputScalarN("Degrees (u,v)", ImGuiDataType_S32, degrees, 2, &step, NULL, "%d");
    UpdateControlPointCounts(ctrl_point_counts[0] - 1, ctrl_point_counts[1] - 1);
    UpdateKnotCounts(degrees[0] + m + 1, degrees[1] + n + 1);
}