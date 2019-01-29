#include "platform.h"
#include "main.h"
// using namespace std;
bool flag=false;

Platform::Platform(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[36];
    createRect(-0.25f, -0.25f, 0.25f, -0.25f, 0.25f, 0.25f, -0.25f, 0.25f, vertex_buffer_data, 1.0f);
    this->object = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data, color, GL_FILL);
}

void Platform::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Platform::tick() {
    if(this->position.x <  CAMERA_X_COR - 5)
    {
        this->position.x = PLATFORM_X_COR;
        if(flag)
        {
            PLATFORM_X_COR += 0.5;
            flag=false;
        }
        else
            flag=true;
    }
}
