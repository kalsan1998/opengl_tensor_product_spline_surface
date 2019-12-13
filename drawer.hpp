#ifndef DRAWER_H
#define DRAWER_H

#include <GL/glut.h>

class Drawer
{
public:
    virtual ~Drawer() = default;
    virtual void Draw() = 0;
    virtual void ProcessKeys(int key, int action) = 0;

protected:
    GLuint program;
    GLuint color_location;
};

#endif