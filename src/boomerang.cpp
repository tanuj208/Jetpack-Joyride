#include "boomerang.h"
#include "main.h"

// using namespace std;

Boomerang::Boomerang(float y, int dir) {
    float x;
    this->active = true;
    if(dir==1)
    {
        this->velx = -8;
        this->accel = 4;
        x = CAMERA_X_COR + SCREEN_RIGHT + 1.0f;
    }
    else
    {
        this->velx = 8;
        this->accel = -4;
        x = CAMERA_X_COR + SCREEN_LEFT - 1.0f;
    }
    if(y>0)
        this->vely = -1;
    else
        this->vely = 1;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat vertex_buffer_data[] = {
        -0.6f, 0.0f, 0.0f,
        0.0f, 0.7f, 0.0f,
        0.0f, 1.2f, 0.0f,
    };
    GLfloat ob2[] = {
        0.0f, 1.2f, 0.0f,
        0.0f, 0.7f, 0.0f,
        0.6f, 0.0f, 0.0f
    };

    this->object = create3DObject(GL_TRIANGLES, 1*3, vertex_buffer_data, {0, 0, 0}, GL_FILL);
    this->ob2 = create3DObject(GL_TRIANGLES, 1*3, ob2, {102, 102, 153}, GL_FILL);
}

void Boomerang::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // cout<<"DLKJ"<<endl;
    draw3DObject(this->object);
    draw3DObject(this->ob2);
    // cout<<"DLKJ"<<endl;
}

void Boomerang::tick() {
    // cout<<"FLJ"<<endl;
    double time_change = (1.0f) / (60.0f);
    this->position.x += this->velx * time_change + (this->accel * time_change * time_change)/2;
    this->velx = this->velx + this->accel * time_change;
    this->position.y += this->vely * time_change;
    if(this->active)
    {
        if(this->position.y < -2)
            this->position.y = -2;
        if(this->position.y > 3)
            this->position.y = 3;
    }
    this->rotation++;
}