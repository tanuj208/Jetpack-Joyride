#include "ring.h"
#include "main.h"

// using namespace std;

Ring::Ring(float y) {
    int n = 50;
    int i,j;
    float x = SCREEN_RIGHT + CAMERA_X_COR + 1.0f;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat vertex_buffer_data[36*n+5];
    double angle = -2*PI / n;
    double x1 = -1.0f;
    double y1 = 0.0f;
    double x2 = -1.1f;
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
            vertex_buffer_data[cntr] = r[j];
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

    this->object = create3DObject(GL_TRIANGLES, 2*n*3, vertex_buffer_data, {0, 102, 153}, GL_FILL);
}

void Ring::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}