#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(bool onGround, bool noGravity);
    double vely;
    double velx;
    double accel;
    bool stuckInRing;
    bool safe;
    string powerUpTaken;
    glm::vec3 ring;
private:
    VAO *object;
    VAO *magnet;
    VAO *shied;
    VAO *face;
};

#endif // BALL_H
