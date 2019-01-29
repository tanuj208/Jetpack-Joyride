#include "main.h"

#ifndef FIREBEAM_H
#define FIREBEAM_H

class FireBeam {
public:
    FireBeam() {}
    FireBeam(float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    bool active;
private:
    VAO *line1;
    VAO *line2;
    VAO *circ1;
    VAO *circ2;
};

#endif
