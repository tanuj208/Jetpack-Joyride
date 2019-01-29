#include "ball.h"
#include "main.h"

// using namespace std;

Ball::Ball(float x, float y) {
    int i;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->vely=0;
    this->velx=0;
    this->accel=ACCEL;
    this->stuckInRing = false;
    this->powerUpTaken = "none";
    int n = 50;
    GLfloat vertex_buffer_data[9*n+5];
    GLfloat shield[] = {
        0.0f, 0.0f, 0.0f,
        0.6f, 0.0f, 0.0f,
        0.6f, 0.25f, 0.0f,
    };
    GLfloat face[108];
    GLfloat tmp[36];
    createRect(-0.25f, -0.2f, -0.25f, -0.3f, 0.25f, -0.3f, 0.25f, -0.2f, face, 0.0f);
    createRect(-0.25f, 0.2f, -0.25f, 0.15f, -0.1f, 0.15f, -0.1f, 0.2f, tmp, 0.0f);
    for(i=0;i<36;i++)
        face[i+36] = tmp[i];
    createRect(0.25f, 0.2f, 0.25f, 0.15f, 0.1f, 0.15f, 0.1f, 0.2f, tmp, 0.0f);
    for(i=0;i<36;i++)
        face[i+72] = tmp[i];

    createPol(0.5f, n, 0.0f, 0.0f, vertex_buffer_data, 0.0f);
    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, {0, 102, 102}, GL_FILL);
    this->face = create3DObject(GL_TRIANGLES, 4*3*3, face, {255, 255, 255}, GL_FILL);
    this->shied = create3DObject(GL_TRIANGLES, 1*3, shield, {255, 153, 51}, GL_FILL);
    this->magnet = create3DObject(GL_TRIANGLES, 1*3, shield, {204, 0, 0}, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->face);

    if(this->powerUpTaken == "shield")
        draw3DObject(this->shied);
    else if(this->powerUpTaken == "magnet")
        draw3DObject(this->magnet);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick(bool onGround, bool noGravity) {
    if(this->powerUpTaken == "shield")
        this->rotation -=80;
    if(this->position.x < SCREEN_LEFT + CAMERA_X_COR + 1)
    {
        if(this->stuckInRing)
        {
            pii tmp = rotate(this->position.x-this->ring.x, this->position.y-this->ring.y + 0.2f, -2*PI/300.0f);
            this->position.x = tmp.first+this->ring.x;
            this->position.y = tmp.second+this->ring.y - 0.2f;
        }
        else
            this->position.x = SCREEN_LEFT + CAMERA_X_COR + 1;
    }
    if((!onGround || (onGround && this->vely >= 0)) && !this->stuckInRing && !noGravity)
    {
        double time_change = (1.0f/60.0f);
        this->position.y += this->vely * time_change + (this->accel * time_change * time_change)/2;
        this->vely = this->vely + this->accel * time_change;
    }
    else if(onGround && this->vely < 0)
    {
        this->position.y = -2.5;
        this->vely = 0;
    }
}

