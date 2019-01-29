#include "coin.h"
#include "main.h"

// using namespace std;

Coin::Coin(float x, float y, color_t color, float score) {
    int i;
    int n = 50;
    this->position = glm::vec3(x, y, 0);
    this->score = score;
    this->rotation = 0;
    GLfloat vertex_buffer_data[9*n+5];
    createPol(COIN_RADIUS, n, 0.0f, 0.0f, vertex_buffer_data, 0.0f);

    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color, GL_FILL);
}

void Coin::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // cout<<this->position.x<<" "<<this->position.y<<endl;
    draw3DObject(this->object);
}
