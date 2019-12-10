#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

class GLFWwindow;

class SphereDrawer
{
public:
    SphereDrawer(GLuint program);
    void ProcessKeys(int key, int action);
    void Draw();

private:
    void LoadVertices();

    GLuint program;
    GLuint color_location;

    float radius;
    int rings;
    int sectors;

    GLuint sphere_vao;
    GLuint sphere_vbo;
    GLuint element_vbo;

    GLenum draw_mode;
};

#endif