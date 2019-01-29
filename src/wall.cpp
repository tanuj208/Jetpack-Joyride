#include "wall.h"
#include "main.h"
// using namespace std;
bool flagg=false;

Wall::Wall(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    int n = 50;
    int i;
    GLfloat vertex_buffer_data[27*n+5];
    GLfloat tmp[9*n+5];
    int cntr=0;
    createPol(1.0f, n, 0.0f, 0.0f, tmp, -2.0f);
    for(i=0;i<9*n;i++) 
    {
        vertex_buffer_data[cntr] = tmp[i];
        cntr++;
    }
    createPol(1.0f, n, 1.0f, 0.0f, tmp, -2.0f);
    for(i=0;i<9*n;i++)
    {
        vertex_buffer_data[cntr] = tmp[i];
        cntr++;
    }
    createPol(1.0f, n, 2.0f, 0.0f, tmp, -2.0f);
    for(i=0;i<9*n;i++) 
    {
        vertex_buffer_data[cntr] = tmp[i];
        cntr++;
    }
    this->object = create3DObject(GL_TRIANGLES, n*9, vertex_buffer_data, {255, 255, 255}, GL_FILL);
}

void Wall::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Wall::tick() {
    if(this->position.x <  CAMERA_X_COR - 5)
    {
        this->position.x = WALL_X_COR;
        if(flagg)
        {
            PLATFORM_X_COR += 5.0f;
            flagg=false;
        }
        else
            flagg=true;
    }
}