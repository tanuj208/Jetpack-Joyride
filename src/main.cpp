#include "main.h"
#include "timer.h"
#include "ball.h"
#include "platform.h"
#include "coin.h"
#include "fireLine.h"
#include "fireBeam.h"
#include "waterBalloon.h"
#include "magnet.h"
#include "boomerang.h"
#include "ring.h"
#include "powerUps.h"
#include "display.h"
#include "wall.h"
#include "dragon.h"

// using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball;
FireBeam firebeam;
Powerup powerup;
vector<Platform> platform;
vector<Coin> coins;
vector<FireLine> firelines;
vector<WaterBalloon> waterBalloons;
vector<WaterBalloon> iceBalls; 
vector<WaterBalloon> jet;
vector<Wall> wall;
Magnet mag;
Boomerang boom;
Dragon dragon;
Ring ring;
Display display[10];

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float top, bottom, leftt, rightt;
float camera_rotation_angle = 0;
double ACCEL = -3;
double VEL_ON_SPACEBAR = 4;
double PI = 3.1415926535897;
double RED_COINS = 1000;
double BLUE_COINS = 700;
double GREEN_COINS = 500;
double COINS_BUNCH = 10;
double COIN_RADIUS = 0.1;
double Frame_vel = 0.5;
double Frame_accel = 0.01;
double CAMERA_X_COR = 0;
double PLATFORM_X_COR;
double WALL_X_COR;
double SCREEN_LEFT = -4.5;
double SCREEN_RIGHT = 4.5;
double SCORE = 0;
bool beamPos;
int numberOfTimes=0;
bool Dead = false;
bool onGround=false;
bool fired=false;
bool noGravity = false;
int powerUpTaken = -1;
color_t COLOR_BACKGROUND = {0, 255, 255};

Timer t60(1.0 / 60);

string sevenSegment(int x)
{
    if(x==0)
        return "1111110";
    else if(x==1)
        return "0110000";
    else if(x==2)
        return "1101101";
    else if(x==3)
        return "1111001";
    else if(x==4)
        return "0110011";
    else if(x==5)
        return "1011011";
    else if(x==6)
        return "1011111";
    else if(x==7)
        return "1110000";
    else if(x==8)
        return "1111111";
    else
        return "1111011";
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    double time_change = (1.0/60);
    CAMERA_X_COR += Frame_vel * time_change + (Frame_accel * time_change * time_change)/2;
    Frame_vel += Frame_accel * time_change;
    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye(CAMERA_X_COR, 0, 20);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!

    // glm::vec3 target (camera_x_cor, 0, 0);
    glm::vec3 target (CAMERA_X_COR, 0, 0);

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

    // Scene render
    int i;
    if(numberOfTimes == 1)
    {
        firebeam = FireBeam(1000);
        mag = Magnet(1000);
        boom = Boomerang(1000, 1);
        boom.active = false;
        ring = Ring(1000);
        powerup = Powerup(1000, 1, "shield");
        dragon =Dragon(1000);
    }

    if(numberOfTimes % 720 == 0)
    {
        double y = (rand() % 6000 - 2500.0f)/1000.0f;
        if(y>0)
            beamPos = true;
        else
            beamPos = false;
        
        firebeam = FireBeam(y);
    }
    else if(numberOfTimes % 720 == 480)
        firebeam = FireBeam(1000);

    if(numberOfTimes % 1100 == 0)
    {
        double y = (rand() % 6000 - 2500.0f)/1000.0f;
        mag = Magnet(y);
    }

    firebeam.position.x = CAMERA_X_COR;

    if(numberOfTimes % 1900 == 0)
    {
        double y = (rand() % 6000 - 2500.0f)/1000.0f;
        int flag = (rand() % 2);
        boom = Boomerang(y, flag);
    }

    if(numberOfTimes % 1400 == 0)
    {
        double y;
        if(numberOfTimes % 1100 == 0)
        {
            if(mag.position.y > 0)
                y = mag.position.y - 2;
            else
                y = mag.position.y + 2;
        }
        else
            y = (rand() % 6000 - 2500.0f)/1000.0f;
        ring = Ring(y);
    }

    if(numberOfTimes % 1300 == 0)
    {
        double y = (rand() % 6000 - 2500.0f)/1000.0f;
        int flag = (rand() % 2);
        int flag2 = (rand()%2);
        if(flag2 == 0)
            powerup = Powerup(y, flag, "shield");
        else
            powerup = Powerup(y, flag, "magnet");
        
    }

    if(numberOfTimes % 2600 == 0)
    {
        double y = (rand() % 6000 - 2500.0f)/1000.0f;
        dragon = Dragon(y);
    }

    vector<ll> sc;
    int tmp = SCORE;
    if(SCORE == 0)
        sc.pb(0);
    while(tmp!=0)
    {
        sc.pb(tmp%10);
        tmp/=10;
    }
    int cntr=0;
    for(i=sz(sc)-1;i>=0;i--)
    {
        display[cntr] = Display(CAMERA_X_COR + leftt + cntr * 0.4f * (1.0f/screen_zoom) + 1.0f/screen_zoom, top - 0.8f/screen_zoom, sevenSegment(sc[i]));
        cntr++;
    }

    if(numberOfTimes % 720 <= 60)
        firebeam.active = false;
    else if(numberOfTimes % 720 <=180)
        firebeam.active = true;
    else if(numberOfTimes % 720 <= 300)
    {
        if(beamPos)
            firebeam.position.y -= 0.01f;
        else
            firebeam.position.y += 0.01f;
        
        firebeam.active = false;
    }
    else if(numberOfTimes % 720 <= 420)
        firebeam.active = true;
    else
        firebeam.active = false;

    if(numberOfTimes % 120 == 0)
        iceBalls.pb(WaterBalloon(dragon.position.x-0.5f, dragon.position.y+0.25, "iceball", -3.0f, 2.0f, {0, 0, 255}, 0.25f));
    
    for(i=0;i<sz(wall);i++)
        wall[i].draw(VP);
    for(i=0;i<sz(platform);i++)
        platform[i].draw(VP);
    for(i=0;i<sz(coins);i++)
        coins[i].draw(VP);
    for(i=0;i<sz(firelines);i++)
        firelines[i].draw(VP);
    for(i=0;i<sz(waterBalloons);i++)
        waterBalloons[i].draw(VP);
    for(i=0;i<sz(iceBalls);i++)
        iceBalls[i].draw(VP);
    ball.draw(VP);
    powerup.draw(VP);
    firebeam.draw(VP);
    mag.draw(VP);
    boom.draw(VP);
    ring.draw(VP);
    for(i=0;i<sz(sc);i++)
        display[i].draw(VP);
    for(i=0;i<sz(jet);i++)
        jet[i].draw(VP);
    dragon.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int spacebar = glfwGetKey(window, GLFW_KEY_SPACE);
    int fire = glfwGetKey(window, GLFW_KEY_F);
    int w = glfwGetKey(window, GLFW_KEY_W);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int s = glfwGetKey(window, GLFW_KEY_S);
    int d = glfwGetKey(window, GLFW_KEY_D);
    if (left) {
        ball.rotation+=3;
        if(ball.stuckInRing)
        {
            if(ball.position.x >= CAMERA_X_COR + SCREEN_LEFT + 1 + 0.05 && ball.position.y >= ring.position.y - 0.2f)
            {
                pii tmp = rotate(ball.position.x - ring.position.x, ball.position.y - ring.position.y + 0.2f, 2*PI/100.0f);
                ball.position.x = tmp.first + ring.position.x;
                ball.position.y = tmp.second + ring.position.y - 0.2f;
            }
        }
        else
        {
            if(ball.position.x >= CAMERA_X_COR + SCREEN_LEFT + 1 + 0.05)
                ball.position.x -= 0.05;
        }
        if(ball.velx > 0)
            ball.velx--;
    }
    if(right) {
        ball.rotation-=3;
        if(ball.stuckInRing)
        {
            if(ball.position.x <= CAMERA_X_COR + SCREEN_RIGHT - 1)
            {
                pii tmp = rotate(ball.position.x-ring.position.x, ball.position.y-ring.position.y + 0.2f, -2 * PI / 50.0f);
                ball.position.x = tmp.first+ring.position.x;
                ball.position.y = tmp.second+ring.position.y - 0.2f;
            }
        }
        else
        {
            if(ball.position.x <= CAMERA_X_COR + SCREEN_RIGHT - 1)
                ball.position.x += 0.05;
        }
        if(ball.velx < 0)
            ball.velx++;
    }
    if(up) {
        if(ball.position.y == -2.5 && !ball.stuckInRing)
            ball.vely = VEL_ON_SPACEBAR;
    }
    if(spacebar) {
        if(ball.position.y <=3.5 && !ball.stuckInRing)
        {
            ball.position.y += 0.1;
            ball.vely = 0;
        }
        if(!ball.stuckInRing)
        {
            int ve = rand() % 5;
            double vee;
            if(ve==0)
                vee=-0.8f;
            else if(ve==1)
                vee=-0.4f;
            else if(ve==2)
                vee=0.0f;
            else if(ve==3)
                vee = 0.4f;
            else if(ve==4)
                vee=0.8f;
            jet.pb(WaterBalloon(ball.position.x, ball.position.y - 0.5f, "jet", vee, 0.0f, {255, 153, 0}, 0.1f));
        }
    }
    if(fire && !fired) {
        waterBalloons.pb(WaterBalloon(ball.position.x + 0.5, ball.position.y, "balloons", 4, 2, {0, 153, 255}, 0.25f));
        fired = true;
    }
    else if(fire == GLFW_RELEASE && fired == true)
        fired=false;
    if(screen_zoom > 1)
    {
        if(w) {
            screen_center_y+= 0.1f;
        }
        if(s) {
            screen_center_y-= 0.1f;
        }
        if(a) {
            screen_center_x -= 0.1f;
        }
        if(d) {
            screen_center_x += 0.1f;
        }
    }
}

void tick_elements() {
    int i,j;
    ball.tick(onGround, noGravity);
    boom.tick();
    powerup.tick();
    dragon.tick();
    for(i=0;i<sz(platform);i++)
        platform[i].tick();
    for(i=0;i<sz(waterBalloons);i++)
        waterBalloons[i].tick();
    for(i=0;i<sz(iceBalls);i++)
        iceBalls[i].tick();
    for(i=0;i<sz(jet);i++)
        jet[i].tick();
    for(i=0;i<sz(coins);i++)
    {
        coins[i].rotation+=2;
        if(coins[i].position.x < CAMERA_X_COR + SCREEN_LEFT - 1)
            coins.erase(coins.begin()+i);
    }
    for(i=0;i<sz(waterBalloons);i++)
        if(waterBalloons[i].position.x < CAMERA_X_COR + SCREEN_LEFT - 1)
            waterBalloons.erase(waterBalloons.begin() + i);
    for(i=0;i<sz(iceBalls);i++)
        if(iceBalls[i].position.x < CAMERA_X_COR + SCREEN_LEFT - 1)
            iceBalls.erase(iceBalls.begin() + i);
    if(mag.position.x <= CAMERA_X_COR + SCREEN_LEFT - 1)
        mag = Magnet(1000);
    camera_rotation_angle += 1;
    if(dist(ball.position.x, ball.position.y, mag.position.x, mag.position.y) < mag.radius_of_effect && !ball.stuckInRing)
    {
        double time_change = (1.0f/60.0f);
        double x1 = ball.position.x - mag.position.x;
        double y1 = ball.position.y - mag.position.y;
        double acx,acy;
        acx = -mag.accel * x1 / sqrt(x1 * x1 + y1 * y1);
        acy = -mag.accel * y1 / sqrt(x1 * x1 + y1 * y1);
        ball.position.y += ball.vely * time_change + (acy * time_change * time_change)/2;
        ball.vely = ball.vely + acy * time_change;
        ball.position.x += ball.velx * time_change + (acx * time_change * time_change)/2;
        ball.velx = ball.velx + acx * time_change;
    }
    else
        ball.velx = 0;
    if(powerUpTaken != -1 && ball.powerUpTaken == "magnet")
    {
        for(i=0;i<sz(coins);i++)
        {
            if(dist(coins[i].position.x, coins[i].position.y, ball.position.x, ball.position.y) <= 2.0f)
            {
                SCORE+=coins[i].score;
                coins.erase(coins.begin() + i);
            }
        }
    }
}

void initCoins() {
    int i,j;
    int x_cor=-2;
    int y_cor=-1;
    for(i=0;i<RED_COINS/COINS_BUNCH;i++)
    {
        x_cor += 3;
        for(j=0;j<COINS_BUNCH;j++)
            coins.pb(Coin(x_cor + 2*j*COIN_RADIUS, y_cor, {192, 192, 192}, 1));
    }

    x_cor = -1;
    y_cor = 1;

    for(i=0;i<GREEN_COINS/COINS_BUNCH;i++)
    {
        x_cor += 4;
        for(j=0;j<COINS_BUNCH;j++)
            coins.pb(Coin(x_cor + 2*j*COIN_RADIUS, y_cor, {0, 102, 0}, 2));
    }

    x_cor = 0;
    y_cor = 3;

    for(i=0;i<BLUE_COINS/COINS_BUNCH;i++)
    {
        x_cor += 6;
        for(j=0;j<COINS_BUNCH;j++)
            coins.pb(Coin(x_cor + 2*j*COIN_RADIUS, y_cor, {0, 51, 204}, 5));
    }
}

void initPlatform() {
    int i;
    double x_cor = -4.5;
    double y_cor = -3.75;
    for(i=0;i<30;i++)
    {
        if(i%2==0)
            platform.pb(Platform(x_cor, y_cor, {102, 0, 51}));
        else
            platform.pb(Platform(x_cor, y_cor, {255, 136, 77}));
        x_cor+=0.5;
    }
    PLATFORM_X_COR = x_cor;
    x_cor = -4.5;
    for(i=0;i<30;i++)
    {
        if(i%2==0)
            platform.pb(Platform(x_cor, y_cor + 0.5, {255, 136, 77}));
        else
            platform.pb(Platform(x_cor, y_cor + 0.5, {102, 0, 51}));
        x_cor+=0.5;
    }
}

void initWall() {
    int i;
    double x_cor = -3.0f;
    double y_cor = 2.0f;
    for(i=0;i<5;i++)
    {
        wall.pb(Wall(x_cor, y_cor));
        x_cor += 5.0f;
    }
    WALL_X_COR = x_cor;
}

void initFireLine() {
    int i;
    double x_cor = 4;
    double y_cor;
    double angle;
    for(i=0;i<200;i++)
    {
        y_cor = (rand() % 6000 - 3000.0f)/1000.0f;
        angle = (rand() % (2 * 3140 )) / 1000.0f;
        // angle = PI/2;
        firelines.pb(FireLine(x_cor, y_cor, angle));
        x_cor += 5;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball = Ball(0.5, -2.5);
    initCoins();
    initPlatform();
    initWall();
    initFireLine();
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
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

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        onGround = false;
        // Process timers

        if (t60.processTick()) {
            reset_screen();
            numberOfTimes++;
            // 60 fps
            // OpenGL Draw command
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            check_collisions();
            tick_elements();
            tick_input(window);
        }
        // if(Dead)
        // {
        //     cout<<"GAME OVER\n";
        //     cout<<"Your score is "<<SCORE<<endl;
        //     break;
        // }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool point_inside_circle(double x1, double y1, double cx1, double cy1, double rad) {
    if((x1-cx1) * (x1-cx1) + (y1-cy1) * (y1-cy1) - (rad * rad) < 0)
        return true;
    else
        return false;
}

bool point_on_line(double x1, double y1, double x2, double y2, double x3, double y3) {
    double len = dist(x1,y1,x2,y2);
    double d1 = dist(x1,y1,x3,y3);
    double d2 = dist(x2,y2,x3,y3);
    if(d1+d2 >= len - 0.1 && d1+d2 <= len+0.1)
        return true;
    else
        return false;
}

bool detect_collision_circle_with_line(pii cirCntr, double rad, pair<pii,pii> endpoints, pii lineCntr) {
    double X = cirCntr.first;
    double Y = cirCntr.second;
    double x1 = endpoints.first.first + lineCntr.first;
    double y1 = endpoints.first.second + lineCntr.second;
    double x2 = endpoints.second.first + lineCntr.first;
    double y2 = endpoints.second.second + lineCntr.second;

    if(point_inside_circle(x1,y1,X,Y,rad) || point_inside_circle(x2,y2,X,Y,rad))
        return true;

    double len  = dist(x1,y1,x2,y2);
    double dot = ( ((X-x1)*(x2-x1)) + ((Y-y1)*(y2-y1)) ) / (len*len);
    double closestX = x1 + (dot * (x2-x1));
    double closestY = y1 + (dot * (y2-y1));

    bool onSegment = point_on_line(x1,y1,x2,y2, closestX,closestY);
    if (!onSegment)
        return false;
    double distX = closestX - X;
    double distY = closestY - Y;
    double distance = sqrt( (distX*distX) + (distY*distY) );
    if(distance <= rad)
        return true;
    return false;
}


void check_collisions() {
    bounding_box_t ballBx = boundingBox(ball.position.x, ball.position.y, 1.0f, 1.0f);

    int i,j;
    for(i=0;i<sz(platform);i++)
    {
        bounding_box_t platBx = boundingBox(platform[i].position.x, platform[i].position.y, 0.5f, 0.5f);
        if(detect_collision(platBx, ballBx))
            onGround = true;
    }
    for(i=0;i<sz(coins);i++)
    {
        bounding_box_t coinBx = boundingBox(coins[i].position.x, coins[i].position.y, 2 * COIN_RADIUS, 2 * COIN_RADIUS);
        if(detect_collision(coinBx, ballBx))
        {
            SCORE += coins[i].score;
            coins.erase(coins.begin() + i);
        }
    }
    for(i=0;i<sz(firelines);i++)
    {
        if(detect_collision_circle_with_line(mp(ball.position.x, ball.position.y), 0.5f, firelines[i].endpoints, mp(firelines[i].position.x, firelines[i].position.y)))
        {
            if(!ball.safe)
                Dead = true;
            firelines.erase(firelines.begin()+i);
        }
    }
    bounding_box_t beam1 = boundingBox(firebeam.position.x, firebeam.position.y + 0.075f, 0.05f, SCREEN_RIGHT - SCREEN_LEFT - 2);
    bounding_box_t beam2 = boundingBox(firebeam.position.x, firebeam.position.y - 0.075f, 0.05f, SCREEN_RIGHT - SCREEN_LEFT - 2);
    if((detect_collision(beam1, ballBx) || detect_collision(beam2, ballBx)) && firebeam.active)
    {
        if(!ball.safe)
            Dead = true;
        firebeam = FireBeam(1000);
    }
    for(i=0;i<sz(waterBalloons);i++)
    {
        bounding_box_t balloonBx = boundingBox(waterBalloons[i].position.x, waterBalloons[i].position.y, 0.5f, 0.5f);
        for(j=0;j<sz(platform);j++)
        {
            bounding_box_t platformBx = boundingBox(platform[j].position.x, platform[j].position.y, 0.5f, 0.5f);
            if(detect_collision(platformBx, balloonBx))
            {
                waterBalloons.erase(waterBalloons.begin() + i);
                break;
            }
        }
        for(j=0;j<sz(firelines);j++)
        {
            if(detect_collision_circle_with_line(mp(waterBalloons[i].position.x, waterBalloons[i].position.y), 0.25f, firelines[j].endpoints, mp(firelines[j].position.x, firelines[j].position.y)))
            {
                waterBalloons.erase(waterBalloons.begin() + i);
                firelines.erase(firelines.begin() + j);
                break;
            }
        }
        if((detect_collision(beam1, balloonBx) || detect_collision(beam2, balloonBx)) && firebeam.active)
        {
            waterBalloons.erase(waterBalloons.begin() + i);
            firebeam = FireBeam(1000);
        }
        for(j=0;j<sz(iceBalls);j++)
        {
            bounding_box_t iceBx = boundingBox(iceBalls[i].position.x, iceBalls[i].position.y, 0.5f, 0.5f);
            if(detect_collision(balloonBx, iceBx))
            {
                waterBalloons.erase(waterBalloons.begin()+i);
                iceBalls.erase(iceBalls.begin()+j);
                break;
            }
        }

    }
    bounding_box_t ringBx = boundingBox(ring.position.x - 1.0f, ring.position.y-0.3f, 0.2f, 0.2f);
    bounding_box_t ring2Bx = boundingBox(ring.position.x + 1.0f, ring.position.y - 0.3f, 0.2f, 0.2f);

    if(detect_collision(ballBx, ringBx))
    {
        if(!ball.stuckInRing)
        {
            ball.position.x = ring.position.x - 1.0f;
            ball.position.y = ring.position.y - 0.4f;
        }
        ball.stuckInRing = true;
        ball.ring.x = ring.position.x;
        ball.ring.y = ring.position.y;
    }
    if(detect_collision(ballBx, ring2Bx))
        ball.stuckInRing = false;
    ball.safe = false;
    if(ball.stuckInRing)
        ball.safe = true;
    if(powerUpTaken != -1 && ball.powerUpTaken == "shield")
        ball.safe = true;

    pii ballCntr = mp(ball.position.x, ball.position.y);
    pii boomCntr = mp(boom.position.x, boom.position.y);
    pair<pii,pii> line1 = mp(rotate(-0.6f, 0.0f, boom.rotation), rotate(0.0f, 0.7f, boom.rotation * PI / 180.0f));
    pair<pii,pii> line2 = mp(rotate(-0.6f, 0.0f, boom.rotation), rotate(0.0f, 1.2f, boom.rotation * PI / 180.0f));
    pair<pii,pii> line3 = mp(rotate(0.6f, 0.0f, boom.rotation), rotate(0.0f, 0.7f, boom.rotation * PI / 180.0f));
    pair<pii,pii> line4 = mp(rotate(0.6f, 0.0f, boom.rotation), rotate(0.0f, 1.2f, boom.rotation * PI / 180.0f));
    // cout<<ball.safe<<endl;
    if(detect_collision_circle_with_line(ballCntr, 0.5f, line1, boomCntr) || detect_collision_circle_with_line(ballCntr, 0.5f, line2, boomCntr) || detect_collision_circle_with_line(ballCntr, 0.5f, line3, boomCntr) || detect_collision_circle_with_line(ballCntr, 0.5f, line4, boomCntr))
    {
        if(!ball.safe)
            Dead = true;
        boom = Boomerang(1000, 1);
        boom.active = false;
    }

    bounding_box_t powerupBx = boundingBox(powerup.position.x, powerup.position.y, 0.6f, 0.6f);
    if(detect_collision(powerupBx, ballBx))
    {
        powerUpTaken = numberOfTimes;
        ball.powerUpTaken = powerup.typ;
        powerup = Powerup(5000, 1, ball.powerUpTaken);
    }
    if(numberOfTimes - powerUpTaken >= 600)
    {
        ball.powerUpTaken = "none";
        powerUpTaken = -1;
    }
    for(i=0;i<sz(iceBalls);i++)
    {
        bounding_box_t iceBx = boundingBox(iceBalls[i].position.x, iceBalls[i].position.y, 0.5f, 0.5f);
        if(detect_collision(iceBx, ballBx))
        {
            // cout<<ball.safe<<endl;
            if(!ball.safe)
                Dead = true;
            iceBalls.erase(iceBalls.begin()+i);
            break;
        }
        for(j=0;j<sz(platform);j++)
        {
            bounding_box_t platBx = boundingBox(platform[i].position.x, platform[i].position.y, 0.5f, 0.5f);
            if(detect_collision(iceBx, platBx))
            {
                iceBalls.erase(iceBalls.begin()+i);
                break;
            }
        }
    }
    for(i=0;i<sz(jet);i++)
    {
        bounding_box_t jetBx = boundingBox(jet[i].position.x, jet[i].position.y, 2*jet[i].rad, 2*jet[i].rad);
        for(j=0;j<sz(platform);j++)
        {
            bounding_box_t platBx = boundingBox(platform[i].position.x, platform[i].position.y, 0.5f, 0.5f);
            if(detect_collision(jetBx, platBx))
            {
                jet.erase(jet.begin()+i);
                break;
            }
        }
    }
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 <= (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 <= (a.height + b.height));
}


void reset_screen() {
    top    = screen_center_y + 4 / screen_zoom;
    bottom = screen_center_y - 4 / screen_zoom;
    leftt   = screen_center_x - 4 / screen_zoom;
    rightt  = screen_center_x + 4 / screen_zoom;
    if(screen_zoom > 1)
    {
        if(top > 4)
            screen_center_y = 4 - 4 / screen_zoom;
        if(bottom <  -4)
            screen_center_y = -4 + 4 / screen_zoom;
        if(leftt < -4)
            screen_center_x = -4 + 4 / screen_zoom;
        if(rightt > 4)
            screen_center_x = 4 - 4 / screen_zoom;
    }
    Matrices.projection = glm::ortho(leftt, rightt, bottom, top, 0.1f, 500.0f);
}

void createRect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, GLfloat vertex_buffer_data[], float z_cor) {
    float cx = (x1+x2+x3+x4)/4;
    float cy = (y1+y2+y3+y4)/4;
    vertex_buffer_data[0] = cx;
    vertex_buffer_data[1] = cy;
    vertex_buffer_data[2] = z_cor;
    vertex_buffer_data[3] = x1;
    vertex_buffer_data[4] = y1;
    vertex_buffer_data[5] = z_cor;
    vertex_buffer_data[6] = x2;
    vertex_buffer_data[7] = y2;
    vertex_buffer_data[8] = z_cor;
    vertex_buffer_data[9] = cx;
    vertex_buffer_data[10] = cy;
    vertex_buffer_data[11] = z_cor;
    vertex_buffer_data[12] = x2;
    vertex_buffer_data[13] = y2;
    vertex_buffer_data[14] = z_cor;
    vertex_buffer_data[15] = x3;
    vertex_buffer_data[16] = y3;
    vertex_buffer_data[17] = z_cor;
    vertex_buffer_data[18] = cx;
    vertex_buffer_data[19] = cy;
    vertex_buffer_data[20] = z_cor;
    vertex_buffer_data[21] = x3;
    vertex_buffer_data[22] = y3;
    vertex_buffer_data[23] = z_cor;
    vertex_buffer_data[24] = x4;
    vertex_buffer_data[25] = y4;
    vertex_buffer_data[26] = z_cor;
    vertex_buffer_data[27] = cx;
    vertex_buffer_data[28] = cy;
    vertex_buffer_data[29] = z_cor;
    vertex_buffer_data[30] = x4;
    vertex_buffer_data[31] = y4;
    vertex_buffer_data[32] = z_cor;
    vertex_buffer_data[33] = x1;
    vertex_buffer_data[34] = y1;
    vertex_buffer_data[35] = z_cor;
    return;
}

void createPol(float radius, int n, float cntx, float cnty, GLfloat vertex_buffer_data[], float z_cor) {
    // vector<GLfloat> vertex_buffer_data;
    GLfloat prevx=radius;
    GLfloat prevy=0.0f;
    double angle=(2.0/n)*PI;
    int cntr=0;
    int i;
    for(i=0;i<3*n;i++)
    {
        if(i%3==0)
        {
            vertex_buffer_data[cntr] = cntx;
            cntr++;
            vertex_buffer_data[cntr] = cnty;
            cntr++;
            vertex_buffer_data[cntr] = z_cor;
            cntr++;
        }
        else if(i%3==1)
        {
            vertex_buffer_data[cntr] = prevx + cntx;
            cntr++;
            vertex_buffer_data[cntr] = prevy + cnty;
            cntr++;
            vertex_buffer_data[cntr] = z_cor;
            cntr++;
        }
        else
        {
            pii newCor = rotate(prevx, prevy, angle);
			prevx=newCor.first;
			prevy=newCor.second;
            vertex_buffer_data[cntr] = prevx + cntx;
            cntr++;
            vertex_buffer_data[cntr] = prevy + cnty;
            cntr++;
            vertex_buffer_data[cntr] = z_cor;
            cntr++;
        }
    }
    return;
}

bounding_box_t boundingBox(float x, float y, float height, float width) {
    bounding_box_t bx;
    bx.x = x;
    bx.y = y;
    bx.height = height;
    bx.width = width;
    return bx;
}

pii rotate(double x, double y, double angle) {
    pii retVal;
    retVal.first = x * cos(angle) - y * sin(angle);
    retVal.second = x * sin(angle) + y * cos(angle);
    return retVal;
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
}