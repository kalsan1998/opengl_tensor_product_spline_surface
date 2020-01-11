#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include <glm/glm.hpp>

class Base;

class KeyboardHandler
{
public:
    KeyboardHandler(Base *base);
    void KeyPress(int key, int action);

private:
    Base *base;
};

#endif