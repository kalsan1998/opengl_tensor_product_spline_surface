#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include <glm/glm.hpp>

class Renderer;

class KeyboardHandler
{
public:
    KeyboardHandler(Renderer *renderer);
    void KeyPress(int key, int action);

private:
    Renderer *renderer;
};

#endif