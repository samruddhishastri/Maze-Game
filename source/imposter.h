#include "main.h"

#ifndef IMPOSTER_H
#define IMPOSTER_H


class Imposter {
public:
    Imposter() {}
    Imposter(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float*);
    void tick();
    void move_left();
    void move_right();
    void move_up();
    void move_down();
    double speed_y;
    double speed_z;
    GLfloat ratio_y;
    GLfloat ratio_z;
private:
    VAO *object;
};

#endif
