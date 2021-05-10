#include "main.h"

#ifndef COINS_H
#define COINS_H


class Coins{
public:
    Coins() {}
    Coins(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float* arr);
    void tick();
    bool collected = false;
    GLfloat ratio_y;
    GLfloat ratio_z;

private:
    VAO *object;
};

#endif