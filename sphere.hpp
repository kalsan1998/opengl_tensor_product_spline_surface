#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
class SphereDrawer
{
public:
    SphereDrawer();
    void SetAttributes(float radi, int r, int s);
    void DrawSphere();

private:
    void LoadVertices();

    float radius;
    int rings;
    int sectors;

    GLuint sphere_vao;
    GLuint sphere_vbo;
};

#endif