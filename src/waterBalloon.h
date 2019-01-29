#include "main.h"

#ifndef WATERBALLOON_H
#define WATERBALLOON_H


class WaterBalloon {
public:
    WaterBalloon() {}
    WaterBalloon(float x, float y, string typ, float velx, float vely, color_t color, float radius);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double velx;
    double vely;
    double rad;
    double accel;
private:
    VAO *object;
};

#endif // BALL_H
