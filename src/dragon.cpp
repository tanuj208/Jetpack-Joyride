#include "dragon.h"
#include "main.h"

// using namespace std;

Dragon::Dragon(float y) {
    float  x = CAMERA_X_COR + SCREEN_RIGHT + 1.0f;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->vely=2;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[36];
    GLfloat face[] = {
        0.0f, 0.0f, 0.0f,
        -0.5f, 0.25f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    createRect(0.0f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.5f, vertex_buffer_data, 0.0f);

    this->object = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data, {0, 51, 0}, GL_FILL);
    this->face = create3DObject(GL_TRIANGLES, 1*3, face, {255, 51, 0}, GL_FILL);
}

void Dragon::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->face);
}

void Dragon::tick() {
    if(this->position.y < -2)
        this->vely = 2;
    if(this->position.y > 3)
        this->vely = -2;
    double time_change = (1.0f) / (60.0f);
    this->position.y += time_change * this->vely;
}
