#include "main.h"

#ifndef FIRELINE_H
#define FIRELINE_H

class FireLine {
public:
    FireLine() {}
    FireLine(float x, float y, double angle);
    glm::vec3 position;
    float rotation;
    pair<pii,pii> endpoints;
    void draw(glm::mat4 VP);
private:
    VAO *line;
    VAO *circ1;
    VAO *circ2;
};

#endif
