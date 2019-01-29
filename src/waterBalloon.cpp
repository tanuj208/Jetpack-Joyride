#include "waterBalloon.h"
#include "main.h"

// using namespace std;

WaterBalloon::WaterBalloon(float x, float y, string typ, float velx, float vely, color_t color, float radius) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->velx=velx;
    this->vely=vely;
    this->accel=ACCEL;
    this->rad = radius;
    int n;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    if(typ=="balloons")
        n=50;
    else
        n=8;
    
    GLfloat vertex_buffer_data[9*n+5];
    createPol(this->rad, n, 0.0f, 0.0f, vertex_buffer_data, 0.0f);

    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color, GL_FILL);
}

void WaterBalloon::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void WaterBalloon::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void WaterBalloon::tick() {
    double time_change = (1.0f/60.0f);
    this->position.y += this->vely * time_change + (this->accel * time_change * time_change)/2;
    this->vely = this->vely + this->accel * time_change;
    this->position.x += this->velx * time_change;
}

