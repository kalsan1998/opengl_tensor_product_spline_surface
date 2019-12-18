#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <GL/glew.h>

#include "drawer.hpp"

class SphereDrawer : public Drawer
{
public:
    SphereDrawer(GLuint program);
    ~SphereDrawer() = default;
    void ProcessKeys(int key, int action) override;
    void Draw() override;
    void GuiLogic() override {}

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