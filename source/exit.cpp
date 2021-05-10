#include "exit.h"
#include "main.h"

Exit::Exit(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    ratio_y = 7;
    ratio_z = 10;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {

       0,(float)1/ratio_y,(float)1/ratio_z,
       0,(float)1/ratio_y,(float)-1/ratio_z,
       0,(float)-1/ratio_y,(float)1/ratio_z,

       0,(float)-1/ratio_y,(float)-1/ratio_z,
       0,(float)1/ratio_y,(float)-1/ratio_z,
       0,(float)-1/ratio_y,(float)1/ratio_z,
    
       0,(float)-1.5/ratio_y,(float)-1/ratio_z,
       0,(float)1.5/ratio_y,(float)-1/ratio_z,
       0, 0,(float)-2/ratio_z,
    };

    this->object = create3DObject(GL_TRIANGLES, 3*3, vertex_buffer_data, color, GL_FILL);
}

void Exit::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Exit::set_position() {
    this->position = glm::vec3(0, -2.75, -3.6);
}

void Exit::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
