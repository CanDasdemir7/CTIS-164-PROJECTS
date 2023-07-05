/*********
   CTIS164 - Template Source Program
----------
STUDENT : MUSTAFACAN DAÞDEMÝR
SECTION :001
HOMEWORK: #2
----------
PROBLEMS
----------
ADDITIONAL FEATURES:
 
  PRESS F1 : Reset the game
  I add some features for gun:  for example you can rotate the gun by using 
  UP/DOWN: AIM GUN and by using LEFT/RIGHT you can MOVE JET
*********/


#define _CRT_SECURE_NO_WARNINGS
#define GL_SILENCE_DEPRECATION
#ifdef _MSC_VER
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>


#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT  800

#define TIMER_PERIOD    16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532       // Degree to Radian conversion


#define FPS 60
#define MAX_FIRE 500
#define SINGLE   1
#define DOUBLE   2

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


typedef struct {
    double x, y;
}point_t;
typedef struct {
    point_t pos;
    double angle;
}player_t;
typedef struct {   //colors and cordinations for bug1
    point_t pos;
    double radius;
    bool hit;
    double x, y;
}bug1_t;
typedef struct {  ////colors and cordinations for bug2
    double x, y;
    bool hit;
}bug2_t;
typedef struct {  ////colors and cordinations for bug2
    double x, y;
    bool hit;
}bug3_t;
typedef struct {  ////colors and cordinations for bug2
    double x, y;
    bool hit;
}bug4_t;
typedef struct {  ////colors and cordinations for bug2
    double x, y;
    bool hit;
}bug5_t;
typedef struct {
    point_t pos;
    float angle;
    bool active;
}fire_t;

player_t p = { {0,-250}, 0 };
bug1_t bug1 = { -10, 500 };
bug2_t bug2 = { 200, 300 };
bug3_t bug3 = { 440, -100 };
bug4_t bug4 = { -350, -50 };
bug5_t bug5 = { -540, false };

fire_t fire[MAX_FIRE];
point_t c;
bool spacebar = false;
int fire_rate = 0,
mode = SINGLE,
FIRERATE = 15,
UfoDif = 3,
bugA = 7,
fSpeed = 8;



//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void vertex(point_t P, point_t Tr, double angle)
{
    float xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    float yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2f(xp, yp);
}

void drawBackground() {


    glColor3ub(108, 29, 124);
    glBegin(GL_POLYGON);
    glVertex2f(-winWidth / 2, winHeight / 2);
    glVertex2f(winWidth / 2, winHeight / 2);
    glVertex2f(winWidth / 2, -winHeight / 2);
    glVertex2f(-winWidth / 2, -winHeight / 2);
    glEnd();

    //****
    glColor3f(255, 255, 255);
    glRectf(-630, 305, 40, 280);
    glColor3ub(255, 0, 0);
    vprint(-484, 284, GLUT_BITMAP_9_BY_15, "<space bar>firing***UP/DOWN:AIM GUN***LEFT/RIGHT:MOVE JET ");
  





}
void drawLabel()
{
    glColor3ub(255, 92, 0);
    vprint(-480, 350, GLUT_BITMAP_9_BY_15, "MUSTAFACAN DASDEMIR 21903303");
}
void Settings() {

    glColor3ub(255, 255, 0);
    vprint(-35, -380, GLUT_BITMAP_9_BY_15, "SPACEBAR: FIRE");
    vprint(150, -380, GLUT_BITMAP_9_BY_15, "UP/DOWN: AIM GUN");
    vprint(320, -380, GLUT_BITMAP_9_BY_15, "LEFT/RIGHT: MOVE JET");

}

void plane() {
    float angle = p.angle * D2R;


    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);

    vertex({ -10 , 40 }, p.pos, angle);
    vertex({ 10, 40 }, p.pos, angle);
    vertex({ +10, -40 }, p.pos, angle);
    vertex({ -10, -40 }, p.pos, angle);
    glEnd();

    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);
    vertex({ -60 , 10 }, p.pos, angle);
    vertex({ 60, 10 }, p.pos, angle);
    vertex({ +60, -10 }, p.pos, angle);
    vertex({ -60, -10 }, p.pos, angle);
    glEnd();

    glBegin(GL_TRIANGLES);
    vertex({ 0 , -30 }, p.pos, angle);
    vertex({ -20, -50 }, p.pos, angle);
    vertex({ 20, -50 }, p.pos, angle);
    glEnd();


    glBegin(GL_TRIANGLES);
    vertex({ 0 , +50 }, p.pos, angle);
    vertex({ -20, 30 }, p.pos, angle);
    vertex({ 20, 30 }, p.pos, angle);
    glEnd();

}

void BUG1()
{


    glColor3ub(9, 193, 0);
    circle(bug1.x, bug1.y, 0);

    //Head of bug
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug1.x + 30, bug1.y + 30);
    glVertex2f(bug1.x - 30, bug1.y + 30);
    glVertex2f(bug1.x - 30, bug1.y - 30);
    glVertex2f(bug1.x + 30, bug1.y - 30);
    glEnd();

    //right Leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug1.x + 30, bug1.y - 20);
    glVertex2f(bug1.x + 70, bug1.y - 20);
    glVertex2f(bug1.x + 70, bug1.y - 30);
    glVertex2f(bug1.x + 30, bug1.y - 30);
    glEnd();


    //right leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug1.x + 60, bug1.y - 30);
    glVertex2f(bug1.x + 70, bug1.y - 30);
    glVertex2f(bug1.x + 70, bug1.y - 70);
    glVertex2f(bug1.x + 60, bug1.y - 70);
    glEnd();

    //left leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug1.x - 70, bug1.y - 20);
    glVertex2f(bug1.x - 30, bug1.y - 20);
    glVertex2f(bug1.x - 30, bug1.y - 30);
    glVertex2f(bug1.x - 70, bug1.y - 30);
    glEnd();

    //left leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug1.x - 70, bug1.y - 30);
    glVertex2f(bug1.x - 60, bug1.y - 30);
    glVertex2f(bug1.x - 60, bug1.y - 70);
    glVertex2f(bug1.x - 70, bug1.y - 70);
    glEnd();


    //middle leg
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug1.x - 5, bug1.y - 30);
    glVertex2f(bug1.x + 5, bug1.y - 30);
    glVertex2f(bug1.x + 5, bug1.y - 60);
    glVertex2f(bug1.x - 5, bug1.y - 60);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);
    glVertex2f(bug1.x + 15, bug1.y + 15);
    glVertex2f(bug1.x + 15, bug1.y - 15);
    glVertex2f(bug1.x - 15, bug1.y - 15);
    glVertex2f(bug1.x - 15, bug1.y + 15);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 0, 0);
    glVertex2f(bug1.x + 5, bug1.y + 5);
    glVertex2f(bug1.x + 5, bug1.y - 5);
    glVertex2f(bug1.x - 5, bug1.y - 5);
    glVertex2f(bug1.x - 5, bug1.y + 5);
    glEnd();

}
void BUG2() {

    glColor3ub(9, 193, 0);
    circle(bug2.x, bug2.y, 1);

    //Head of bug
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug2.x + 30, bug2.y + 30);
    glVertex2f(bug2.x - 30, bug2.y + 30);
    glVertex2f(bug2.x - 30, bug2.y - 30);
    glVertex2f(bug2.x + 30, bug2.y - 30);
    glEnd();

    //right Leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug2.x + 30, bug2.y - 20);
    glVertex2f(bug2.x + 70, bug2.y - 20);
    glVertex2f(bug2.x + 70, bug2.y - 30);
    glVertex2f(bug2.x + 30, bug2.y - 30);
    glEnd();


    //right leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug2.x + 60, bug2.y - 30);
    glVertex2f(bug2.x + 70, bug2.y - 30);
    glVertex2f(bug2.x + 70, bug2.y - 70);
    glVertex2f(bug2.x + 60, bug2.y - 70);
    glEnd();

    //left leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug2.x - 70, bug2.y - 20);
    glVertex2f(bug2.x - 30, bug2.y - 20);
    glVertex2f(bug2.x - 30, bug2.y - 30);
    glVertex2f(bug2.x - 70, bug2.y - 30);
    glEnd();

    //left leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug2.x - 70, bug2.y - 30);
    glVertex2f(bug2.x - 60, bug2.y - 30);
    glVertex2f(bug2.x - 60, bug2.y - 70);
    glVertex2f(bug2.x - 70, bug2.y - 70);
    glEnd();


    //middle leg
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug2.x - 5, bug2.y - 30);
    glVertex2f(bug2.x + 5, bug2.y - 30);
    glVertex2f(bug2.x + 5, bug2.y - 60);
    glVertex2f(bug2.x - 5, bug2.y - 60);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);
    glVertex2f(bug2.x + 15, bug2.y + 15);
    glVertex2f(bug2.x + 15, bug2.y - 15);
    glVertex2f(bug2.x - 15, bug2.y - 15);
    glVertex2f(bug2.x - 15, bug2.y + 15);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 0, 0);
    glVertex2f(bug2.x + 5, bug2.y + 5);
    glVertex2f(bug2.x + 5, bug2.y - 5);
    glVertex2f(bug2.x - 5, bug2.y - 5);
    glVertex2f(bug2.x - 5, bug2.y + 5);
    glEnd();


}


void BUG3()
{


    glColor3ub(9, 193, 0);
    circle(bug3.x, bug3.y, 0);

    //Head of bug
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug3.x + 30, bug3.y + 30);
    glVertex2f(bug3.x - 30, bug3.y + 30);
    glVertex2f(bug3.x - 30, bug3.y - 30);
    glVertex2f(bug3.x + 30, bug3.y - 30);
    glEnd();

    //right Leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug3.x + 30, bug3.y - 20);
    glVertex2f(bug3.x + 70, bug3.y - 20);
    glVertex2f(bug3.x + 70, bug3.y - 30);
    glVertex2f(bug3.x + 30, bug3.y - 30);
    glEnd();


    //right leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug3.x + 60, bug3.y - 30);
    glVertex2f(bug3.x + 70, bug3.y - 30);
    glVertex2f(bug3.x + 70, bug3.y - 70);
    glVertex2f(bug3.x + 60, bug3.y - 70);
    glEnd();

    //left leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug3.x - 70, bug3.y - 20);
    glVertex2f(bug3.x - 30, bug3.y - 20);
    glVertex2f(bug3.x - 30, bug3.y - 30);
    glVertex2f(bug3.x - 70, bug3.y - 30);
    glEnd();

    //left leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug3.x - 70, bug3.y - 30);
    glVertex2f(bug3.x - 60, bug3.y - 30);
    glVertex2f(bug3.x - 60, bug3.y - 70);
    glVertex2f(bug3.x - 70, bug3.y - 70);
    glEnd();


    //middle leg
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug3.x - 5, bug3.y - 30);
    glVertex2f(bug3.x + 5, bug3.y - 30);
    glVertex2f(bug3.x + 5, bug3.y - 60);
    glVertex2f(bug3.x - 5, bug3.y - 60);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);
    glVertex2f(bug3.x + 15, bug3.y + 15);
    glVertex2f(bug3.x + 15, bug3.y - 15);
    glVertex2f(bug3.x - 15, bug3.y - 15);
    glVertex2f(bug3.x - 15, bug3.y + 15);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 0, 0);
    glVertex2f(bug3.x + 5, bug3.y + 5);
    glVertex2f(bug3.x + 5, bug3.y - 5);
    glVertex2f(bug3.x - 5, bug3.y - 5);
    glVertex2f(bug3.x - 5, bug3.y + 5);
    glEnd();

}

void BUG4()
{


    glColor3ub(9, 193, 0);
    circle(bug4.x, bug4.y, 0);

    //Head of bug
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug4.x + 30, bug4.y + 30);
    glVertex2f(bug4.x - 30, bug4.y + 30);
    glVertex2f(bug4.x - 30, bug4.y - 30);
    glVertex2f(bug4.x + 30, bug4.y - 30);
    glEnd();

    //right Leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug4.x + 30, bug4.y - 20);
    glVertex2f(bug4.x + 70, bug4.y - 20);
    glVertex2f(bug4.x + 70, bug4.y - 30);
    glVertex2f(bug4.x + 30, bug4.y - 30);
    glEnd();


    //right leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug4.x + 60, bug4.y - 30);
    glVertex2f(bug4.x + 70, bug4.y - 30);
    glVertex2f(bug4.x + 70, bug4.y - 70);
    glVertex2f(bug4.x + 60, bug4.y - 70);
    glEnd();

    //left leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug4.x - 70, bug4.y - 20);
    glVertex2f(bug4.x - 30, bug4.y - 20);
    glVertex2f(bug4.x - 30, bug4.y - 30);
    glVertex2f(bug4.x - 70, bug4.y - 30);
    glEnd();

    //left leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug4.x - 70, bug4.y - 30);
    glVertex2f(bug4.x - 60, bug4.y - 30);
    glVertex2f(bug4.x - 60, bug4.y - 70);
    glVertex2f(bug4.x - 70, bug4.y - 70);
    glEnd();


    //middle leg
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug4.x - 5, bug4.y - 30);
    glVertex2f(bug4.x + 5, bug4.y - 30);
    glVertex2f(bug4.x + 5, bug4.y - 60);
    glVertex2f(bug4.x - 5, bug4.y - 60);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);
    glVertex2f(bug4.x + 15, bug4.y + 15);
    glVertex2f(bug4.x + 15, bug4.y - 15);
    glVertex2f(bug4.x - 15, bug4.y - 15);
    glVertex2f(bug4.x - 15, bug4.y + 15);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 0, 0);
    glVertex2f(bug4.x + 5, bug4.y + 5);
    glVertex2f(bug4.x + 5, bug4.y - 5);
    glVertex2f(bug4.x - 5, bug4.y - 5);
    glVertex2f(bug4.x - 5, bug4.y + 5);
    glEnd();

}

void BUG5()
{


    glColor3ub(9, 193, 0);
    circle(bug5.x, bug5.y, 0);

    //Head of bug
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug5.x + 30, bug5.y + 30);
    glVertex2f(bug5.x - 30, bug5.y + 30);
    glVertex2f(bug5.x - 30, bug5.y - 30);
    glVertex2f(bug5.x + 30, bug5.y - 30);
    glEnd();

    //right Leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug5.x + 30, bug5.y - 20);
    glVertex2f(bug5.x + 70, bug5.y - 20);
    glVertex2f(bug5.x + 70, bug5.y - 30);
    glVertex2f(bug5.x + 30, bug5.y - 30);
    glEnd();


    //right leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug5.x + 60, bug5.y - 30);
    glVertex2f(bug5.x + 70, bug5.y - 30);
    glVertex2f(bug5.x + 70, bug5.y - 70);
    glVertex2f(bug5.x + 60, bug5.y - 70);
    glEnd();

    //left leg first
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug5.x - 70, bug5.y - 20);
    glVertex2f(bug5.x - 30, bug5.y - 20);
    glVertex2f(bug5.x - 30, bug5.y - 30);
    glVertex2f(bug5.x - 70, bug5.y - 30);
    glEnd();

    //left leg second
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug5.x - 70, bug5.y - 30);
    glVertex2f(bug5.x - 60, bug5.y - 30);
    glVertex2f(bug5.x - 60, bug5.y - 70);
    glVertex2f(bug5.x - 70, bug5.y - 70);
    glEnd();


    //middle leg
    glBegin(GL_POLYGON);
    glColor3ub(9, 193, 0);
    glVertex2f(bug5.x - 5, bug5.y - 30);
    glVertex2f(bug5.x + 5, bug5.y - 30);
    glVertex2f(bug5.x + 5, bug5.y - 60);
    glVertex2f(bug5.x - 5, bug5.y - 60);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);
    glVertex2f(bug5.x + 15, bug5.y + 15);
    glVertex2f(bug5.x + 15, bug5.y - 15);
    glVertex2f(bug5.x - 15, bug5.y - 15);
    glVertex2f(bug5.x - 15, bug5.y + 15);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 0, 0);
    glVertex2f(bug5.x + 5, bug5.y + 5);
    glVertex2f(bug5.x + 5, bug5.y - 5);
    glVertex2f(bug5.x - 5, bug5.y - 5);
    glVertex2f(bug5.x - 5, bug5.y + 5);
    glEnd();

}
//****


void Fires() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fire[i].active && mode == SINGLE) {
            glColor3ub(0, 249, 255);
            circle(fire[i].pos.x, fire[i].pos.y, 5);
        }

    }
}

int AvailableFire() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fire[i].active == false) return i;
    }
    return -1;
}

bool bug1_hit(fire_t fire, bug1_t bug1) {
    if (fire.pos.x <= bug1.x + 90 && fire.pos.x >= bug1.x &&
        fire.pos.y >= -5 && fire.pos.y <= 17)
        return true;
    else
        return false;
}

bool bug2_hit(fire_t fire, bug2_t bug2) {
    if (fire.pos.x <= bug2.x + 90 && fire.pos.x >= bug2.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
}
bool bug3_hit(fire_t fire, bug3_t bug3) {
    if (fire.pos.x <= bug3.x + 90 && fire.pos.x >= bug3.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
}
bool bug4_hit(fire_t fire, bug4_t bug4) {
    if (fire.pos.x <= bug4.x + 90 && fire.pos.x >= bug4.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
}
bool bug5_hit(fire_t fire, bug5_t bug5) {
    if (fire.pos.x <= bug5.x + 90 && fire.pos.x >= bug5.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    drawLabel();
    Fires();
    plane();
    Settings();
   
    if (!bug1.hit) BUG1();
    if (!bug2.hit) BUG2();
    if (!bug3.hit) BUG3();
    if (!bug4.hit) BUG4();
    if (!bug5.hit) BUG5();

    if (bug1.hit == 1 && bug2.hit == 1 && bug3.hit == 1 && bug4.hit == 1 && bug5.hit == 1)
    {
        glColor3ub(255, 0, 254);
        vprint2(-300, -50, 0.2, "        PRESS F1 TO RESTART");

    }
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    switch (key) {
    case ' ': spacebar = true; break;
        mode = SINGLE;

    }

    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ')
        spacebar = false;
    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y) {
    // Write your codes here.
    switch (key) {

    case GLUT_KEY_UP: up = true; break;
    
    case GLUT_KEY_DOWN: down = true; break;
    
    case GLUT_KEY_LEFT: left = true; break;
    
    case GLUT_KEY_RIGHT: right = true; break;
    
    case GLUT_KEY_F1:
        if (bug2.hit == true)
            bug2.x = -240;
        bug1.hit = false;
        bug2.hit = false;
        bug3.hit = false;
        bug4.hit = false;
        bug5.hit = false;
        break;
    }

    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }


    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   mx = x - winWidth / 2;
//   my = winHeight / 2 - y;
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.


    // Moving ship and Aimin the gun
    if (right) p.pos.x += 18;
    if (left) p.pos.x -= 18;
    if (down) p.angle += 3;
    if (up) p.angle -= 3;

    if (p.pos.x + 40 >= winWidth / 2) p.pos.x -= 5;
    if (p.pos.x - 40 <= -winWidth / 2) p.pos.x += 5;
    if (p.angle > 45)p.angle -= 3;
    if (p.angle < -45)p.angle += 3;

    bug5.x += bugA;
    if (bug5.x >= winWidth / 2)
        bug5.x = -winWidth / 2 - 120;

    bug4.x += bugA;
    if (bug4.x >= winWidth / 2)
        bug4.x = -winWidth / 2 - 120;

    bug3.x += bugA;
    if (bug3.x >= winWidth / 2)
        bug3.x = -winWidth / 2 - 120;

    bug2.x += bugA;
    if (bug2.x >= winWidth / 2)
        bug2.x = -winWidth / 2 - 120;

    bug1.x += bugA;
    if (bug1.x >= winWidth / 2)
        bug1.x = -winWidth / 2 - 120;

    if (spacebar && fire_rate == 0) {
        int availablefire = AvailableFire();
        if (availablefire != -1) {
            fire[availablefire].pos = p.pos;
            fire[availablefire].angle = p.angle + 90;
            fire[availablefire].active = true;
            fire_rate = FIRERATE;
        }
    }

    if (fire_rate > 0)
        fire_rate--;

    for (int i = 0; i < MAX_FIRE; i++) {
        if (fire[i].active)
        {
            fire[i].pos.x += fSpeed * cos(fire[i].angle * D2R);
            fire[i].pos.y += fSpeed * sin(fire[i].angle * D2R);

            if (bug1_hit(fire[i], bug1))
                bug1.hit = true;

            if (bug2_hit(fire[i], bug2))
                bug2.hit = true;

            if (bug3_hit(fire[i], bug3))
                bug3.hit = true;

            if (bug4_hit(fire[i], bug4))
                bug4.hit = true;

            if (bug5_hit(fire[i], bug5))
                bug5.hit = true;

        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(250, 120);
    glutCreateWindow("HW2: \"Destroying Bugs");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
