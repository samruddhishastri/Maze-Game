#include "main.h"
#include "timer.h"
#include "player.h"
#include "maze.h"
#include "exit.h"
#include "imposter.h"
#include "task.h"
#include "bombs.h"
#include "coins.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Player player1;
Maze maze1;
Exit exit1;
Imposter imposter1;
Task task1;
Task task2;
Bombs all_bombs[3];
Coins all_coins[5];
int t = 0;

bool lights = true;
float time_sec;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

bool task1_completed = false;
bool task2_completed = false;

int last_move = 1;
char str[100];
int health = 10;
int total_tasks = 2;
int total_time = 150;

int screen = 0;

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    if(screen == 0){
        // Scene render
        maze1.draw(VP);
        exit1.draw(VP);
        if(task1_completed == false){
            task1.draw(VP);
            imposter1.draw(VP);
        }
        if(task2_completed == true){
            for(int i=0; i<5; i++){
                if(all_coins[i].collected == false)
                    all_coins[i].draw(VP);
            }
        }
        else{
            task2.draw(VP);
            for(int i=0; i<3; i++){
                if(all_bombs[i].exploded == false)
                    all_bombs[i].draw(VP);
            }
        }

        player1.draw(VP);
    }
}

void you_won(){
    screen = 1;
}

void you_lost(){
    screen = 2;
}


void check_task1(){
    if(abs(task1.position.z-player1.position.z) <= 0.10 && abs(task1.position.y-player1.position.y) <= 0.10){
        task1_completed = true;
        total_tasks--;
    }
}

void check_task2(){
    if(abs(task2.position.z-player1.position.z) <= 0.10 && abs(task2.position.y-player1.position.y) <= 0.10){
        task2_completed = true;
        total_tasks--;
    }
}

void check_collision_with_bomb(){
    for(int i=0; i<3; i++){
        if(abs(all_bombs[i].position.z-player1.position.z) <= 0.10 && abs(all_bombs[i].position.y-player1.position.y) <= 0.10 && (all_bombs[i].exploded == false)){
            all_bombs[i].exploded = true;
            health-=2;
        }
    }
}

void check_collision_coin(){
    for(int i=0; i<5; i++){
        if(abs(all_coins[i].position.z-player1.position.z) <= 0.10 && abs(all_coins[i].position.y-player1.position.y) <= 0.10 && (all_coins[i].collected == false)){
            all_coins[i].collected = true;
            health++;
        }
    }
}

void check_collision_with_imposter(){
    if(abs(imposter1.position.z-player1.position.z) <= 0.10 && abs(imposter1.position.y-player1.position.y) <= 0.10){
        you_lost();
    }
}

void check_exit(){
    if(task1_completed && task2_completed){
        if(abs(exit1.position.z-player1.position.z) <= 0.10 && abs(exit1.position.y-player1.position.y) <= 0.10){
            you_won();
        }
    }
    if(time_sec >= total_time || health <= 0){
        you_lost();
    }
}

void tick_input(GLFWwindow *window) {

    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    if (left) {
        bool x = detect_collision(1);
        if(x == false)
            player1.move_left();
    }
    else if(right){
        bool x = detect_collision(2);
        if(x == false)
            player1.move_right();
    }
    else if(up){
        bool x = detect_collision(3);
        if(x == false)
            player1.move_up();
    }
    else if(down){
        bool x = detect_collision(4);
        if(x == false)
            player1.move_down();
    }
    if(!task1_completed){
	    check_task1();
        check_collision_with_imposter();
    }
	if(!task2_completed){
        check_task2();
        check_collision_with_bomb();
    }
    if(task2_completed){
        check_collision_coin();
    }
	check_exit();
}

void move_imposter(){
    bool a = detect_collision_imposter(1);
    bool b = detect_collision_imposter(2);
    bool c = detect_collision_imposter(3);
    bool d = detect_collision_imposter(4);

	if(last_move == 1){
		if(a == false){
            imposter1.move_left();
            last_move = 1;
            return;
        }
        if(c == false && d == false){
            int t = rand()%2;
            if(t==1){
                imposter1.move_up();
                last_move = 3;
            }
			else{
                imposter1.move_down();
                last_move = 4;
            }
            return;  
        }
        if(c == false){
            imposter1.move_up();
            last_move = 3;
            return;
        }
        if(d == false){
            imposter1.move_down();
            last_move = 4;
            return;
        }
        else
            imposter1.move_right();
            last_move = 2;
            return;
	}

	if(last_move == 2){
        if(b == false){
            imposter1.move_right();
            last_move = 2;
            return;
        }
		if(c == false && d == false){
            int t = rand()%2;
            if(t==1){
                imposter1.move_up();
                last_move = 3;
            }
			else{
                imposter1.move_down();
                last_move = 4;
            }
            return;  
        }
        if(c == false){
            imposter1.move_up();
            last_move = 3;
            return;
        }
        if(d == false){
            imposter1.move_down();
            last_move = 4;
            return;
        }
        else
            imposter1.move_left();
            last_move = 1;
            return;
	}

    if(last_move == 3){
		if(c == false){
            imposter1.move_up();
            last_move = 3;
            return;
        }
        if(a == false && b == false){
            int t = rand()%2;
            if(t==1){
                imposter1.move_left();
                last_move = 1;
            }
			else{
                imposter1.move_right();
                last_move = 2;
            }
            return;  
        }
        if(a == false){
            imposter1.move_left();
            last_move = 1;
            return;
        }
        if(b == false){
            imposter1.move_right();
            last_move = 2;
            return;
        }
        else
            imposter1.move_down();
            last_move = 4;
            return;
	}

    if(last_move == 4){
		if(d == false){
            imposter1.move_down();
            last_move = 4;
            return;
        }
        if(a == false && b == false){
            int t = rand()%2;
            if(t==1){
                imposter1.move_left();
                last_move = 1;
            }
			else{
                imposter1.move_right();
                last_move = 2;
            }
            return;  
        }
        if(a == false){
            imposter1.move_left();
            last_move = 1;
            return;
        }
        if(b == false){
            imposter1.move_right();
            last_move = 2;
            return;
        }
        else
            imposter1.move_up();
            last_move = 3;
            return;
	}
}

void tick_elements() {
    player1.tick();
    maze1.tick();
    exit1.tick();
    imposter1.tick();
    task1.tick();
	task2.tick();
	if(task1_completed==false && t%30==0)
	    move_imposter();
    t++;
    //camera_rotation_angle += 1;
}

void create_bombs(float** arr){
    for(int i=0; i<3; i++){
        all_bombs[i] = Bombs(0,0, COLOR_BLACK);
        all_bombs[i].set_position(arr[i]);
    }
}

void create_coins(float** arr){
    for(int i=0; i<5; i++){
        all_coins[i] = Coins(0,0, COLOR_YELLOW);
        all_coins[i].set_position(arr[i]);
    }
}
/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    player1     = Player(0, 0, COLOR_RED);
    maze1       = Maze(0, 0, COLOR_BROWN);
    exit1       = Exit(0, 0, COLOR_RED);
    imposter1   = Imposter(0, 0, COLOR_RED);
    task1       = Task(0, 0, COLOR_GREEN); 
	task2		= Task(0, 0, COLOR_RED);

    player1.set_position();
    float* arr = maze1.get_coordinates_for_imposter();
    float* arr1 = maze1.get_coordinates_for_task1();
	float* arr2 = maze1.get_coordinates_for_task2();
    float** arr3 = maze1.get_coordinates_for_bombs();
    float** arr4 = maze1.get_coordinates_for_coins();

    exit1.set_position();
    imposter1.set_position(arr);
    task1.set_position(arr1);
	task2.set_position(arr2);
    create_bombs(arr3);
    create_coins(arr4);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    if (!gltInit())
	{
		fprintf(stderr, "Failed to initialize glText\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

    GLTtext *title = gltCreateText();
	gltSetText(title, "MAZE GAME");

	GLTtext *hud = gltCreateText();

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        time_sec = glfwGetTime();

        if (t60.processTick()) {
            if(screen == 0){
            
                draw();

                gltBeginDraw();

                gltColor(
                    cosf((float)time_sec) * 0.5f + 0.5f,
                    sinf((float)time_sec) * 0.5f + 0.5f,
                    1.0f,
                    1.0f);

                gltDrawText2DAligned(title, width*1.5, 0.0f, 3.0f, GLT_CENTER, GLT_TOP); // x=0.0, y=0.0, scale=1.0

                sprintf(str, "Time:%.0f      Health:%d       Tasks:%d/2        Lights:On", total_time - time_sec, health, total_tasks);
                gltSetText(hud, str);

                gltColor(
                    1.0f,
                    1.0f,
                    1.0f,
                    1.0f);

                gltDrawText2DAligned(hud, width*1.5, 75.0f, 2.0f, GLT_CENTER, GLT_TOP); // x=0.0, y=0.0, scale=1.0

                gltColor(1.0f, 1.0f, 1.0f,1.0f);

                gltEndDraw();

                // Swap Frame Buffer in double buffering
                glfwSwapBuffers(window);

                tick_elements();
                tick_input(window);
            }

            else if(screen == 1){
                draw();
                gltSetText(title, "YOU WON! :)");

                gltBeginDraw();

                gltColor(
                    cosf((float)time_sec) * 0.5f + 0.5f,
                    sinf((float)time_sec) * 0.5f + 0.5f,
                    1.0f,
                    1.0f);

                gltDrawText2DAligned(title, width*1.5, 500.0f, 5.0f, GLT_CENTER, GLT_CENTER); // x=0.0, y=0.0, scale=1.0

                gltEndDraw();

                // Swap Frame Buffer in double buffering
                glfwSwapBuffers(window);

                tick_input(window);
            }

            else if(screen == 2){
                draw();

                gltSetText(title, "YOU LOST :/ Better luck next time!");

                gltBeginDraw();

                gltColor(
                    cosf((float)time_sec) * 0.5f + 0.5f,
                    sinf((float)time_sec) * 0.5f + 0.5f,
                    1.0f,
                    1.0f);

                gltDrawText2DAligned(title, width*1.5, 500.0f, 5.0f, GLT_CENTER, GLT_CENTER); // x=0.0, y=0.0, scale=1.0

                gltEndDraw();

                // Swap Frame Buffer in double buffering
                glfwSwapBuffers(window);

                tick_input(window);
            }
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

	gltTerminate();
    quit(window);
}

bool detect_collision(int x) {

    if(x==1){
        for(int i=0; i<maze1.wall_v.size(); i++){
            if((maze1.wall_v[i].first.second - player1.position.z < 0.17) && (maze1.wall_v[i].first.second - player1.position.z > 0) && (maze1.wall_v[i].first.first >= player1.position.y) && (maze1.wall_v[i].second.first <= player1.position.y)){
                return true;
            }
            if((maze1.wall_v[i].first.second - player1.position.z < 0.17) && (maze1.wall_v[i].first.second - player1.position.z > 0) && (maze1.wall_v[i].first.first >= player1.position.y) && (maze1.wall_v[i].second.first - player1.position.y <= 0.15)){
                //std::cout<<"called"<<std::endl;
                return true;
            }
            if((maze1.wall_v[i].first.second - player1.position.z < 0.17) && (maze1.wall_v[i].first.second - player1.position.z > 0) && (player1.position.y - maze1.wall_v[i].first.first <= 0.15) && (maze1.wall_v[i].second.first <= player1.position.y)){
                //std::cout<<"called"<<std::endl;
                return true;
            }
        }
    }
    if(x==2){
        for(int i=0; i<maze1.wall_v.size(); i++){
            if((player1.position.z - maze1.wall_v[i].first.second < 0.17) && (maze1.wall_v[i].first.second - player1.position.z < 0) && (maze1.wall_v[i].first.first >= player1.position.y) && (maze1.wall_v[i].second.first <= player1.position.y)){
                return true;
            }
            if((player1.position.z - maze1.wall_v[i].first.second < 0.17) && (maze1.wall_v[i].first.second - player1.position.z < 0) && (maze1.wall_v[i].first.first >= player1.position.y) && (maze1.wall_v[i].second.first - player1.position.y <= 0.15)){
                return true;
            }
            if((player1.position.z - maze1.wall_v[i].first.second < 0.17) && (maze1.wall_v[i].first.second - player1.position.z < 0) && (player1.position.y - maze1.wall_v[i].first.first <= 0.15) && (maze1.wall_v[i].second.first <= player1.position.y)){
                return true;
            }
        }
    }
    if(x==3){
        for(int i=0; i<maze1.wall_h.size(); i++){
            if((maze1.wall_h[i].first.first - player1.position.y < 0.3) && (maze1.wall_h[i].first.first - player1.position.y > 0) && (maze1.wall_h[i].first.second >= player1.position.z) && (maze1.wall_h[i].second.second <= player1.position.z)){
                return true;
            }
        }
    }
    if(x==4){
        for(int i=0; i<maze1.wall_h.size(); i++){
            if((player1.position.y - maze1.wall_h[i].first.first < 0.3) && (maze1.wall_h[i].first.first - player1.position.y < 0) && (maze1.wall_h[i].first.second >= player1.position.z) && (maze1.wall_h[i].second.second <= player1.position.z)){
                return true;
            }
        }
    }
    return false;
}

bool detect_collision_imposter(int x) {

    if(x==1){
        for(int i=0; i<maze1.wall_v.size(); i++){
            if((maze1.wall_v[i].first.second - imposter1.position.z < 0.33) && (maze1.wall_v[i].first.second - imposter1.position.z > 0) && (maze1.wall_v[i].first.first >= imposter1.position.y) && (maze1.wall_v[i].second.first <= imposter1.position.y)){
                return true;
            }
            if((maze1.wall_v[i].first.second - imposter1.position.z < 0.33) && (maze1.wall_v[i].first.second - imposter1.position.z > 0) && (maze1.wall_v[i].first.first >= imposter1.position.y) && (maze1.wall_v[i].second.first - imposter1.position.y <= 0.15)){
                //std::cout<<"called"<<std::endl;
                return true;
            }
            if((maze1.wall_v[i].first.second - imposter1.position.z < 0.33) && (maze1.wall_v[i].first.second - imposter1.position.z > 0) && (imposter1.position.y - maze1.wall_v[i].first.first <= 0.15) && (maze1.wall_v[i].second.first <= imposter1.position.y)){
                //std::cout<<"called"<<std::endl;
                return true;
            }
        }
    }
    if(x==2){
        for(int i=0; i<maze1.wall_v.size(); i++){
            if((imposter1.position.z - maze1.wall_v[i].first.second < 0.33) && (maze1.wall_v[i].first.second - imposter1.position.z < 0) && (maze1.wall_v[i].first.first >= imposter1.position.y) && (maze1.wall_v[i].second.first <= imposter1.position.y)){
                return true;
            }
            if((imposter1.position.z - maze1.wall_v[i].first.second < 0.33) && (maze1.wall_v[i].first.second - imposter1.position.z < 0) && (maze1.wall_v[i].first.first >= imposter1.position.y) && (maze1.wall_v[i].second.first - imposter1.position.y <= 0.15)){
                return true;
            }
            if((imposter1.position.z - maze1.wall_v[i].first.second < 0.33) && (maze1.wall_v[i].first.second - imposter1.position.z < 0) && (imposter1.position.y - maze1.wall_v[i].first.first <= 0.15) && (maze1.wall_v[i].second.first <= imposter1.position.y)){
                return true;
            }
        }
    }
    if(x==3){
        for(int i=0; i<maze1.wall_h.size(); i++){
            if((maze1.wall_h[i].first.first - imposter1.position.y < 0.5) && (maze1.wall_h[i].first.first - imposter1.position.y > 0) && (maze1.wall_h[i].first.second >= imposter1.position.z) && (maze1.wall_h[i].second.second <= imposter1.position.z)){
                return true;
            }
        }
    }
    if(x==4){
        for(int i=0; i<maze1.wall_h.size(); i++){
            if((imposter1.position.y - maze1.wall_h[i].first.first < 0.5) && (maze1.wall_h[i].first.first - imposter1.position.y < 0) && (maze1.wall_h[i].first.second >= imposter1.position.z) && (maze1.wall_h[i].second.second <= imposter1.position.z)){
                return true;
            }
        }
    }
    return false;
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

