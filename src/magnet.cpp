#include "magnet.h"
#include "main.h"

// using namespace std;

Magnet::Magnet(float y) {
    float x = CAMERA_X_COR + SCREEN_RIGHT + 1.0f;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius_of_effect = 4.0f;
    this->accel = abs(ACCEL) + 0.5;
    int n = 50;
    int i,j=0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    // vector<GLfloat> tmp = createPol(0.5f, n, 0.0f, 0.0f);
    // vector<GLfloat> tmp = createRect(-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f);
    GLfloat vertex_buffer_data[36*n+5];
    GLfloat rect1[36];
    GLfloat rect2[36];
    GLfloat exRect1[36];
    GLfloat exRect2[36];
    createRect(-0.3f, 0.0f, -0.55f, 0.0f, -0.55f, -0.2f, -0.3f, -0.2f, rect1, 0.0f);
    createRect(0.3f, 0.0f, 0.55f, 0.0f, 0.55f, -0.2f, 0.3f, -0.2f, rect2, 0.0f);
    createRect(-0.3f, -0.2f, -0.55f, -0.2f, -0.55f, -0.4f, -0.3f, -0.4f, exRect1, 0.0f);
    createRect(0.3f, -0.2f, 0.55f, -0.2f, 0.55f, -0.4f, 0.3f, -0.4f, exRect2, 0.0f);
    double angle = -2*PI / n;
    double x1 = -0.3f;
    double y1 = 0.0f;
    double x2 = -0.55f;
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

    this->object = create3DObject(GL_TRIANGLES, 2*n*3, vertex_buffer_data, {204, 0, 0}, GL_FILL);
    this->rect1 = create3DObject(GL_TRIANGLES, 4*3, rect1, {204, 0, 0}, GL_FILL);
    this->rect2 = create3DObject(GL_TRIANGLES, 4*3, rect2, {204, 0, 0}, GL_FILL);
    this->exRect1 = create3DObject(GL_TRIANGLES, 4*3, exRect1, {102, 0, 51}, GL_FILL);
    this->exRect2 = create3DObject(GL_TRIANGLES, 4*3, exRect2, {102, 0, 51}, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->rect1);
    draw3DObject(this->rect2);
    draw3DObject(this->exRect1);
    draw3DObject(this->exRect2);
}
