#include "display.h"
#include "main.h"

// using namespace std;

Display::Display(float x, float y,  string number) {
    int i,j;
    this->position = glm::vec3(x, y, 0);
    this->number = number;
    this->rotation = 0;
    GLfloat ob1[36];
    GLfloat ob2[36];
    GLfloat ob3[36];
    GLfloat ob4[36];
    GLfloat ob5[36];
    GLfloat ob6[36];
    GLfloat ob7[36];
    createRect(-0.1f, 0.2f, -0.1f, 0.23f, 0.1f, 0.23f, 0.1f, 0.2f, ob1, 0.0f);
    createRect(0.1f, 0.2f, 0.13f, 0.2f, 0.13f, 0.0f, 0.1f, 0.0f, ob2, 0.0f);
    createRect(0.1f, -0.2f, 0.13f, -0.2f, 0.13f, 0.0f, 0.1f, 0.0f, ob3, 0.0f);
    createRect(-0.1f, -0.2f, -0.1f, -0.23f, 0.1f, -0.23f, 0.1f, -0.2f, ob4, 0.0f);
    createRect(-0.1f, -0.2f, -0.13f, -0.2f, -0.13f, 0.0f, -0.1f, 0.0f, ob5, 0.0f);
    createRect(-0.1f, 0.2f, -0.13f, 0.2f, -0.13f, 0.0f, -0.1f, 0.0f, ob6, 0.0f);
    createRect(-0.1f, 0.0f, -0.1f, 0.03f, 0.1f, 0.03f, 0.1f, 0.0f, ob7, 0.0f);
    this->ob1 = create3DObject(GL_TRIANGLES, 4*3, ob1, {0, 0, 0}, GL_FILL);
    this->ob2 = create3DObject(GL_TRIANGLES, 4*3, ob2, {0, 0, 0}, GL_FILL);
    this->ob3 = create3DObject(GL_TRIANGLES, 4*3, ob3, {0, 0, 0}, GL_FILL);
    this->ob4 = create3DObject(GL_TRIANGLES, 4*3, ob4, {0, 0, 0}, GL_FILL);
    this->ob5 = create3DObject(GL_TRIANGLES, 4*3, ob5, {0, 0, 0}, GL_FILL);
    this->ob6 = create3DObject(GL_TRIANGLES, 4*3, ob6, {0, 0, 0}, GL_FILL);
    this->ob7 = create3DObject(GL_TRIANGLES, 4*3, ob7, {0, 0, 0}, GL_FILL);
}

void Display::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 scale = glm::scale((glm::vec3(1.0f/screen_zoom, 1.0f/screen_zoom, 1)));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if(this->number[0]=='1')
        draw3DObject(this->ob1);
    if(this->number[1]=='1')
        draw3DObject(this->ob2);
    if(this->number[2]=='1')
        draw3DObject(this->ob3);
    if(this->number[3]=='1')
        draw3DObject(this->ob4);
    if(this->number[4]=='1')
        draw3DObject(this->ob5);
    if(this->number[5]=='1')
        draw3DObject(this->ob6);
    if(this->number[6]=='1')
        draw3DObject(this->ob7);
}
