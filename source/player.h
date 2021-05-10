#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position();
    void tick();
    void move_left();
    void move_right();
    void move_up();
    void move_down();
    double speed;
    double speed_y;
    GLfloat ratio_y;
    GLfloat ratio_z;
private:
    VAO *object;
};

#endif
