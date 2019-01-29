#include "main.h"

#ifndef COIN_H
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y, color_t color, float score);
    glm::vec3 position;
    float score;
    float rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif
