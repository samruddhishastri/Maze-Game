#include "main.h"

#ifndef MAZE_H
#define MAZE_H


class Maze {
public:
    Maze() {}
    Maze(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void make_maze();
    float* get_coordinates_for_task1();
    float* get_coordinates_for_imposter();
    float* get_coordinates_for_task2();
    float** get_coordinates_for_bombs();
    float** get_coordinates_for_coins();
    double speed;
    std::vector< std::pair< std::pair<GLfloat,GLfloat>, std::pair<GLfloat,GLfloat> > > wall_h;
    std::vector< std::pair< std::pair<GLfloat,GLfloat>, std::pair<GLfloat,GLfloat> > > wall_v;
private:
    VAO *object;
};

#endif
