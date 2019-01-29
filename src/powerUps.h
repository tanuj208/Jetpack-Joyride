#include "main.h"

#ifndef POWERUPS_H
#define POWERUPS_H


class Powerup {
public:
    Powerup() {}
    Powerup(float y, int dir, string typ);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick();
    double vel;
    double velx;
    double accel;
    double meanPos;
    string typ;
private:
    VAO *object;
    VAO *mag;
    VAO *rect1;
    VAO *rect2;
    VAO *exRect1;
    VAO *exRect2;
};

#endif
