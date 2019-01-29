#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet {
public:
    Magnet() {}
    Magnet(float y);
    glm::vec3 position;
    float rotation;
    double radius_of_effect;
    double accel;
    void draw(glm::mat4 VP);
private:
    VAO *object;
    VAO *exRect1;
    VAO *exRect2;
    VAO *rect1;
    VAO *rect2;
};

#endif
