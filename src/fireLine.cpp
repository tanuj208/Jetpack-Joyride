#include "fireLine.h"
#include "main.h"

// using namespace std;

FireLine::FireLine(float x, float y, double angle) {
    this->position = glm::vec3(x, y, 0);
    int n = 50;
    this->rotation = 0;
    double x1,x2,x3,x4,x5,x6,y1,y2,y3,y4,y5,y6;
    pii tmp = rotate(-2.0f, 0.05f, angle);
    x1 = tmp.first;
    y1 = tmp.second;

    tmp = rotate(-2.0f, -0.05f, angle);
    x2 = tmp.first;
    y2 = tmp.second;

    tmp = rotate(2.0f, -0.05f, angle);
    x3 = tmp.first;
    y3 = tmp.second;

    tmp = rotate(2.0f, 0.05f, angle);
    x4 = tmp.first;
    y4 = tmp.second;

    tmp = rotate(-2.0f, 0.0f, angle);
    x5 = tmp.first;
    y5 = tmp.second;

    tmp = rotate(2.0f, 0.0f, angle);
    x6 = tmp.first;
    y6 = tmp.second;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat line_data[36];
    GLfloat circ1_data[9*n+5];
    GLfloat circ2_data[9*n+5];
    createRect(x1, y1, x2, y2, x3, y3, x4, y4, line_data, 0.0f);
    createPol(0.2, n, x5, y5, circ1_data, 0.0f);
    createPol(0.2, n, x6, y6, circ2_data, 0.0f);

    this->endpoints = mp(mp(x5,y5), mp(x6,y6));
    this->line = create3DObject(GL_TRIANGLES, 4*3, line_data, {255, 255, 0}, GL_FILL);
    this->circ1 = create3DObject(GL_TRIANGLES, n*3, circ1_data, {0, 0, 0}, GL_FILL);
    this->circ2 = create3DObject(GL_TRIANGLES, n*3, circ2_data, {0, 0, 0}, GL_FILL);
}

void FireLine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->line);
    draw3DObject(this->circ1);
    draw3DObject(this->circ2);
}
