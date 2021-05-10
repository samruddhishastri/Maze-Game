#include "main.h"

#ifndef EXIT_H
#define EXIT_H


class Exit{
public:
    Exit() {}
    Exit(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position();
    void tick();
    double speed;
    GLfloat ratio_y;
    GLfloat ratio_z;

private:
    VAO *object;
};

#endif
