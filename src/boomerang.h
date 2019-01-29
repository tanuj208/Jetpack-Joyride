#include "main.h"

#ifndef BOOMERANG_H
#define BOOMERANG_H


class Boomerang {
public:
    Boomerang() {}
    Boomerang(float y, int dir);
    glm::vec3 position;
    float rotation;
    double accel;
    double velx,vely;
    bool active;
    void tick();
    void draw(glm::mat4 VP);
private:
    VAO *object;
    VAO *ob2;
};

#endif
