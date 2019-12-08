#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

class GLFWwindow;

class SphereDrawer
{
public:
    SphereDrawer();
    void ProcessKeys(int key, int action);
    void SetAttributes(float radi, int r, int s);
    void DrawSphere();

private:
    void LoadVertices();

    float radius;
    int rings;
    int sectors;

    GLuint sphere_vao;
    GLuint sphere_vbo;
    GLuint element_vbo;

    GLenum draw_mode;
};

#endif