#include "fireBeam.h"
#include "main.h"

// using namespace std;

FireBeam::FireBeam(float y) {
    this->position = glm::vec3(CAMERA_X_COR, y, 0);
    int n = 50;
    this->rotation = 0;
    this->active = false;
    double x1= SCREEN_LEFT + 1;
    double x2 = SCREEN_RIGHT - 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat line1_data[36];
    GLfloat line2_data[36];
    GLfloat circ1_data[9*n+5];
    GLfloat circ2_data[9*n+5];
    createRect(x1, 0.05f, x1, 0.1f, x2, 0.1f, x2, 0.05f, line1_data, 0.0f);
    createRect(x1, -0.05f, x1, -0.1f, x2, -0.1f, x2, -0.05f, line2_data, 0.0f);
    createPol(0.3, n, x1, 0, circ1_data, 0.0f);
    createPol(0.3, n, x2, 0, circ2_data, 0.0f);

    this->line1 = create3DObject(GL_TRIANGLES, 4*3, line1_data, {255, 153, 51}, GL_FILL);
    this->line2 = create3DObject(GL_TRIANGLES, 4*3, line2_data, {255, 153, 51}, GL_FILL);
    this->circ1 = create3DObject(GL_TRIANGLES, n*3, circ1_data, {0, 0, 0}, GL_FILL);
    this->circ2 = create3DObject(GL_TRIANGLES, n*3, circ2_data, {0, 0, 0}, GL_FILL);
}

void FireBeam::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if(this->active)
    {
        draw3DObject(this->line1);
        draw3DObject(this->line2);
    }
    draw3DObject(this->circ1);
    draw3DObject(this->circ2);
}
