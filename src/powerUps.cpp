#include "powerUps.h"
#include "main.h"

// using namespace std;

Powerup::Powerup(float y, int dir, string typ) {
    float x;
    int i,j;
    if(dir==1)
    {
        this->velx = -1.0f;
        x = CAMERA_X_COR + SCREEN_RIGHT + 1.0f;
    }
    else
    {
        this->velx = 1.0f;
        x = CAMERA_X_COR + SCREEN_LEFT - 1.0f;
    }
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->vel=1;
    this->typ = typ;
    if(y>0)
        this->meanPos = y-1;
    else
        this->meanPos = y+1;
    int n = 50;
    GLfloat vertex_buffer_data[9*n+5];
    GLfloat mag[36*n+5];
    GLfloat rect1[36];
    GLfloat rect2[36];
    GLfloat exRect1[36];
    GLfloat exRect2[36];
    createPol(0.3f, n, 0.0f, 0.0f, vertex_buffer_data, 0.0f);
    if(this->typ == "magnet")
    {
        createRect(-0.1f, 0.0f, -0.25f, 0.0f, -0.25f, -0.1f, -0.1f, -0.1f, rect1, 0.0f);
        createRect(0.1f, 0.0f, 0.25f, 0.0f, 0.25f, -0.1f, 0.1f, -0.1f, rect2, 0.0f);
        createRect(-0.1f, -0.1f, -0.25f, -0.1f, -0.25f, -0.25f, -0.1f, -0.25f, exRect1, 0.0f);
        createRect(0.1f, -0.1f, 0.25f, -0.1f, 0.25f, -0.25f, 0.1f, -0.25f, exRect2, 0.0f);
        double angle = -2*PI / n;
        double x1 = -0.1f;
        double y1 = 0.0f;
        double x2 = -0.25f;
        double y2 = 0.0f;
        double x3,y3,x4,y4;
        pii tmp = rotate(x1,y1,angle);
        x3 = tmp.first;
        y3 = tmp.second;

        tmp = rotate(x2,y2,angle);
        x4 = tmp.first;
        y4 = tmp.second;
        int cntr = 0;

        for(i=0;i<n;i++)
        {
            GLfloat r[36];
            createRect(x1,y1,x2,y2,x4,y4,x3,y3,r, 0.0f);
            for(j=0;j<36;j++)
            {
                mag[cntr] = r[j];
                cntr++;
            }
            x1=x3;
            y1=y3;
            x2=x4;
            y2=y4;
            tmp = rotate(x1,y1,angle);
            x3=tmp.first;
            y3=tmp.second;

            tmp=rotate(x2,y2,angle);
            x4=tmp.first;
            y4=tmp.second;
        }

        this->mag = create3DObject(GL_TRIANGLES, 2*n*3, mag, {204, 0, 0}, GL_FILL);
        this->rect1 = create3DObject(GL_TRIANGLES, 4*3, rect1, {204, 0, 0}, GL_FILL);
        this->rect2 = create3DObject(GL_TRIANGLES, 4*3, rect2, {204, 0, 0}, GL_FILL);
        this->exRect1 = create3DObject(GL_TRIANGLES, 4*3, exRect1, {102, 0, 51}, GL_FILL);
        this->exRect2 = create3DObject(GL_TRIANGLES, 4*3, exRect2, {102, 0, 51}, GL_FILL);
    }
    else if(this->typ == "shield")
    {
        GLfloat tr[] = {
            0.0f, -0.2f, 0.0f,
            -0.2f, 0.1f, 0.0f,
            0.0f, 0.2f, 0.0f,
            0.0f, -0.2f, 0.0f,
            0.2f, 0.1f, 0.0f,
            0.0f, 0.2f, 0.0f,
        };
        this->rect1 = create3DObject(GL_TRIANGLES, 2*3, tr, {51, 153, 255}, GL_FILL);
    }
    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, {0, 0, 102}, GL_FILL);
}

void Powerup::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    if(this->typ == "magnet")
    {
        draw3DObject(this->mag);
        draw3DObject(this->rect1);
        draw3DObject(this->rect2);
        draw3DObject(this->exRect1);
        draw3DObject(this->exRect2);
    }
    else if(this->typ == "shield")
        draw3DObject(this->rect1);
}

void Powerup::tick() {
    double time_change = (1.0f) / (60.0f);
    if(this->position.y > this->meanPos)
        this->accel = -1.0f;
    else
        this->accel = 1.0f;
    this->position.y += this->vel * time_change + (this->accel * time_change * time_change)/2.0f;
    this->vel += this->accel * time_change;
    this->position.x += this->velx * time_change;
}

