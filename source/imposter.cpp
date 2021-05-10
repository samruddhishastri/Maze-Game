#include "imposter.h"
#include "main.h"

Imposter::Imposter(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed_z = 0.33;
    speed_y = 0.5;
    ratio_y = 6;
    ratio_z = 10;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {

        //body
        0,(float)-1/ratio_y,(float)-0.7/ratio_z,
        0,(float)1/ratio_y,(float)1/ratio_z,
        0,(float)-1/ratio_y,(float)1/ratio_z,

        0,(float)1/ratio_y,(float)1/ratio_z,
        0,(float)-1/ratio_y,(float)-0.7/ratio_z,
        0,(float)1/ratio_y,(float)-0.7/ratio_z,
        
        //back
        0,(float)0.75/ratio_y,(float)1/ratio_z,
        0,(float)-0.75/ratio_y,(float)1/ratio_z,
        0,(float)0.75/ratio_y,(float)1.25/ratio_z,
        
        0,(float)-0.75/ratio_y,(float)1/ratio_z,
        0,(float)0.75/ratio_y,(float)1.25/ratio_z,
        0,(float)-0.75/ratio_y,(float)1.25/ratio_z,

        //leg1
        0,(float)-1/ratio_y,(float)0.5/ratio_z,
        0,(float)-1.2/ratio_y,(float)0.5/ratio_z,
        0,(float)-1/ratio_y,(float)1/ratio_z,

        0,(float)-1.2/ratio_y,(float)1/ratio_z,
        0,(float)-1.2/ratio_y,(float)0.5/ratio_z,
        0,(float)-1/ratio_y,(float)1/ratio_z,

        //leg2
        0,(float)-1/ratio_y,(float)-0.2/ratio_z,
        0,(float)-1.2/ratio_y,(float)-0.2/ratio_z,
        0,(float)-1/ratio_y,(float)-0.7/ratio_z,

        0,(float)-1.2/ratio_y,(float)-0.7/ratio_z,
        0,(float)-1.2/ratio_y,(float)-0.2/ratio_z,
        0,(float)-1/ratio_y,(float)-0.7/ratio_z,

        //eyeglass
        0,(float)0.7/ratio_y,(float)0.2/ratio_z,
        0,(float)0.2/ratio_y,(float)0.2/ratio_z,
        0,(float)0.7/ratio_y,(float)-1/ratio_z,

        0,(float)0.2/ratio_y,(float)0.2/ratio_z,
        0,(float)0.7/ratio_y,(float)-1/ratio_z,
        0,(float)0.2/ratio_y,(float)-1/ratio_z,

    };

    static const GLfloat color_buffer_data[] = {
        //body
        0,1,0,
        0,1,0,
        0,1,0,

        0,1,0,
        0,1,0,
        0,1,0,
        
        //back
        0,1,0,
        0,1,0,
        0,1,0,
        
        0,1,0,
        0,1,0,
        0,1,0,

        //leg1
        0,1,0,
        0,1,0,
        0,1,0,

        0,1,0,
        0,1,0,
        0,1,0,

        //leg2
        0,1,0,
        0,1,0,
        0,1,0,

        0,1,0,
        0,1,0,
        0,1,0,

        //eyeglass
        0,1,1,
        0,1,1,
        0,1,1,

        0,1,1,
        0,1,1,
        0,1,1,
    };

    this->object = create3DObject(GL_TRIANGLES, 10*3, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void Imposter::draw(glm::mat4 VP) {
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

void Imposter::set_position(float* arr) {
    this->position = glm::vec3(0, arr[0]+0.25, (arr[3]+arr[1])/2);
    if(arr[0] >= 2.5){
        this->position = glm::vec3(0, arr[0]-0.25, (arr[3]+arr[1])/2);
    }
    //std::cout<<arr[0]<<std::endl;
}

void Imposter::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

void Imposter::move_left(){
    this->position.z += speed_z;
}

void Imposter::move_right(){
    this->position.z -= speed_z;
}

void Imposter::move_up(){
    this->position.y += speed_y;
}

void Imposter::move_down(){
    this->position.y -= speed_y;
}
