#include <bits/stdc++.h>
#include "maze.h"
#include "main.h"

int vis[101][101] = {0};
std::string ver[101][101];
std::string hor[101][101];
std::vector<char> allowed_moves[101][101];

GLfloat vec[10004];
int b = 0;
float arr[4], arr1[4], arr2[4];
float** bombs_position = new float*[3];
float** coins_position = new float*[5];

void make_maze();
void walk(int x, int y);


Maze::Maze(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    wall_h.push_back(std::make_pair(std::make_pair(0,0), std::make_pair(0,0)));
    wall_v.push_back(std::make_pair(std::make_pair(0,0), std::make_pair(0,0)));

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    make_maze();
    this->object = create3DObject(GL_LINES, b/3, vec, color, GL_FILL);
    
}

void Maze::draw(glm::mat4 VP) {
    
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

void Maze::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Maze::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

float* Maze::get_coordinates_for_task1(){
    return arr1;
}

float* Maze::get_coordinates_for_task2(){
    return arr2;
}

float* Maze::get_coordinates_for_imposter(){
    return arr;
} 

float** Maze::get_coordinates_for_bombs(){
    return bombs_position;
} 

float** Maze::get_coordinates_for_coins(){
    return coins_position;
} 

void walk(int x,int y){
    vis[x][y] = 1;
    std::pair<int,int> d[4] = {std::make_pair(x-1,y), std::make_pair(x,y+1), std::make_pair(x+1,y), std::make_pair(x,y-1)};
    random_shuffle(d, d + 4);

    for(int i=0; i<4; i++){
        if(d[i].first < 0 || d[i].second < 0)
            continue;
        if(vis[d[i].first][d[i].second] == 1)
            continue;
        if(d[i].first == x){
            hor[x][std::max(y,d[i].second)] = "+  ";
        }
        if(d[i].second == y){
            ver[std::max(x, d[i].first)][y] = "   ";
        }
        walk(d[i].first, d[i].second);
    }
}

void Maze::make_maze(){
    srand(unsigned(time(0)));
    int w = 20, h = 12;

    for(int i=0; i<h; i++){
        for(int j=0; j<w+1; j++){
            ver[j][i] = "|  ";
            hor[j][i] = "+--";
        }
    }

    for(int i=0; i<h; i++){
        vis[w][i] = 1;
        ver[w][i] = "|";
        hor[w][i] = "+";
    }
    
    for(int i=0; i<w+1; i++){
        vis[i][h] = 1;
        hor[i][h] = "+--";
    }
    hor[w][h] = "+";

    walk(2,2);

    int l=0, v=0, ratio_y = 2, ratio_z = 3, diff_y = 3, diff_z = 3;

    for(int i=0; i<h+1; i++){
        l = 0;

        for(int j=0; j<w+1; j++){
            std::cout<<hor[j][i];
            if(hor[j][i] == "+--"){
                vec[b++] = 0;
                vec[b++] = -(float)v/ratio_y + diff_y;
                vec[b++] = -(float)l/ratio_z + diff_z;
                vec[b++] = 0;
                vec[b++] = -(float)v/ratio_y + diff_y;
                vec[b++] = -(float)(l+1)/ratio_z + diff_z;
                wall_h.push_back( std::make_pair (std::make_pair(-(float)v/ratio_y + diff_y, -(float)l/ratio_z + diff_z), std::make_pair(-(float)v/ratio_y + diff_y, -(float)(l+1)/ratio_z + diff_z)));
            }
            l++;
        }

        l = 0;

        std::cout<<"\n";
        for(int j=0; j<w+1; j++){
            std::cout<<ver[j][i];
            if(ver[j][i] == "|  " || ver[j][i] == "|"){
                vec[b++] = 0;
                vec[b++] = -(float)v/ratio_y + diff_y;
                vec[b++] = -(float)l/ratio_z + diff_z;
                vec[b++] = 0;
                vec[b++] = -(float)(v+1)/ratio_y + diff_y;
                vec[b++] = -(float)l/ratio_z + diff_z;
                wall_v.push_back(std::make_pair(std::make_pair(-(float)v/ratio_y + diff_y, -(float)l/ratio_z + diff_z), std::make_pair(-(float)(v+1)/ratio_y + diff_y, -(float)l/ratio_z + diff_z)));
            }
            l++;
        }
        std::cout<<"\n";
        v++;
    }

    int p = rand()%wall_h.size();
    while(p==0 || p==1 || p >= 150){
        p = rand()%wall_h.size();
    }
    arr[0] = wall_h[p].first.first;
    arr[1] = wall_h[p].first.second;
    arr[2] = wall_h[p].second.first;
    arr[3] = wall_h[p].second.second;

    int q = rand()%wall_h.size();
    while(q==p || q==0 || q==1 || q >= 150){
        q = rand()%wall_h.size();
    }

    //std::cout<<arr[0]<<" "<<arr[1]<<" "<<arr[2]<<" "<<arr[3]<<std::endl;

    arr1[0] = wall_h[q].first.first;
    arr1[1] = wall_h[q].first.second;
    arr1[2] = wall_h[q].second.first;
    arr1[3] = wall_h[q].second.second;

    int r = rand()%wall_h.size();

    while(r==q || r==p || r==0 || r==1 || r >= 150){
        r = rand()%wall_h.size();
    }

    arr2[0] = wall_h[r].first.first;
    arr2[1] = wall_h[r].first.second;
    arr2[2] = wall_h[r].second.first;
    arr2[3] = wall_h[r].second.second;

    int b1 = rand()%wall_h.size();
    while(b1==p || b1==q || b1==r || b1==0 || b1==1 || b1 >= 150){
        b1 = rand()%wall_h.size();
    }

    bombs_position[0] = new float[4];
    bombs_position[0][0] = wall_h[b1].first.first;
    bombs_position[0][1] = wall_h[b1].first.second;
    bombs_position[0][2] = wall_h[b1].second.first;
    bombs_position[0][3] = wall_h[b1].second.second;

    int b2 = rand()%wall_h.size();
    while(b2==p || b2==q || b2==r || b2==b1 || b2==0 || b2==1 || b2 >= 150){
        b2 = rand()%wall_h.size();
    }
    
    bombs_position[1] = new float[4];
    bombs_position[1][0] = wall_h[b2].first.first;
    bombs_position[1][1] = wall_h[b2].first.second;
    bombs_position[1][2] = wall_h[b2].second.first;
    bombs_position[1][3] = wall_h[b2].second.second;

    int b3 = rand()%wall_h.size();
    while(b3==p || b3==q || b3==r || b3==b1 || b3==b2 || b3==0 || b3==1 || b3 >= 150){
        b3 = rand()%wall_h.size();
    }

    bombs_position[2] = new float[4];

    bombs_position[2][0] = wall_h[b3].first.first;
    bombs_position[2][1] = wall_h[b3].first.second;
    bombs_position[2][2] = wall_h[b3].second.first;
    bombs_position[2][3] = wall_h[b3].second.second;

    int c1 = rand()%wall_h.size();
    while(c1==p || c1==q || c1==r || c1==b1 || c1==b2 || c1 == b3 || c1==0 || c1==1 || c1 >= 150){
        c1 = rand()%wall_h.size();
    }

    coins_position[0] = new float[4];

    coins_position[0][0] = wall_h[c1].first.first;
    coins_position[0][1] = wall_h[c1].first.second;
    coins_position[0][2] = wall_h[c1].second.first;
    coins_position[0][3] = wall_h[c1].second.second;

    int c2 = rand()%wall_h.size();
    while(c2==p || c2==q || c2==r || c2==b1 || c2==b2 || c2 == b3 || c2 == c1 || c2==0 || c2==1 || c2 >= 150){
        c2 = rand()%wall_h.size();
    }

    coins_position[1] = new float[4];

    coins_position[1][0] = wall_h[c2].first.first;
    coins_position[1][1] = wall_h[c2].first.second;
    coins_position[1][2] = wall_h[c2].second.first;
    coins_position[1][3] = wall_h[c2].second.second;

    int c3 = rand()%wall_h.size();
    while(c3==p || c3==q || c3==r || c3==b1 || c3==b2 || c3==b3 || c3==c1 || c3==c2 || c3==0 || c3==1 || c3 >= 150){
        c3 = rand()%wall_h.size();
    }

    coins_position[2] = new float[4];

    coins_position[2][0] = wall_h[c3].first.first;
    coins_position[2][1] = wall_h[c3].first.second;
    coins_position[2][2] = wall_h[c3].second.first;
    coins_position[2][3] = wall_h[c3].second.second;

    int c4 = rand()%wall_h.size();
    while(c4==p || c4==q || c4==r || c4==b1 || c4==b2 || c4==b3 || c4==c1 || c4==c2 || c4==c3 || c4==0 || c4==1 || c4 >= 150){
        c4 = rand()%wall_h.size();
    }

    coins_position[3] = new float[4];

    coins_position[3][0] = wall_h[c4].first.first;
    coins_position[3][1] = wall_h[c4].first.second;
    coins_position[3][2] = wall_h[c4].second.first;
    coins_position[3][3] = wall_h[c4].second.second;

    int c5 = rand()%wall_h.size();
    while(c5==p || c5==q || c5==r || c5==b1 || c5==b2 || c5==b3 || c5==c1 || c5==c2 || c5==c3 || c5==c4 || c5==0 || c5==1 || c5 >= 150){
        c5 = rand()%wall_h.size();
    }

    coins_position[4] = new float[4];

    coins_position[4][0] = wall_h[c5].first.first;
    coins_position[4][1] = wall_h[c5].first.second;
    coins_position[4][2] = wall_h[c5].second.first;
    coins_position[4][3] = wall_h[c5].second.second;

}
