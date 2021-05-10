#include "main.h"

#ifndef TASK_H
#define TASK_H


class Task {
public:
    Task() {}
    Task(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float *);
    void tick();
    void activate();
    double speed;
private:
    VAO *object;
};

#endif
