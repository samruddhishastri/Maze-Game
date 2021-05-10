#include "main.h"

#ifndef BOMBS_H
#define BOMBS_H


class Bombs{
public:
    Bombs() {}
    Bombs(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float* arr);
    void tick();
    bool exploded = false;
    GLfloat ratio_y;
    GLfloat ratio_z;

private:
    VAO *object;
};

#endif
