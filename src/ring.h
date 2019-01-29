#include "main.h"

#ifndef RING_H
#define RING_H


class Ring {
public:
    Ring() {}
    Ring(float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif
