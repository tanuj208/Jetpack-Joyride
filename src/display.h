#include "main.h"

#ifndef DISPLAY_H
#define DISPLAY_H


class Display {
public:
    Display() {}
    Display(float x, float y, string number);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    string number;
private:
    VAO *ob1;
    VAO *ob2;
    VAO *ob3;
    VAO *ob4;
    VAO *ob5;
    VAO *ob6;
    VAO *ob7;
};

#endif
