/*

CTIS164 Homework
----------
STUDENT : Mustafacan Daþdemir
SECTION : 02
HOMEWORK: #1
Additional feautes:
I add a certain speed to the clouds and make them move, so it's a more realistic view.
----------
*/


#define _CRT_SECURE_NO_WARNINGS
#define GL_SILENCE_DEPRECATION
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT  600

#define TIMER_PERIOD    16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R      0.0174532

// Modes
#define STOP             0
#define ACTION           1

// Shape Frames
#define COLOUR1          1
#define COLOUR2          2
#define COLOUR3          3
#define COLOUR4          4
#define COLOUR5          5
#define COLOUR6          6

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int mx, my;               // Current position of mouse pointer
int xShape, yShape;       // Assigned shape locations
int mode = STOP;          // ACTION: Start Moving, STOP: stop
int cMode = COLOUR1;      // Colour Modes
bool Timer = false; // timer
int sCnt = 0;             // Shape count
int r = 0;

double xC = 0; //cloud initial position

typedef struct {
    int count,
        sx, sy;           // Shape Coordinates
}shape_t;

shape_t Shape[10];


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

void display_background() {

    //Sky
    glColor3ub(52, 181, 255);
    glBegin(GL_QUADS);
    glVertex2f(-500, 550);
    glVertex2f(500, 550);
    glVertex2f(500, -150);
    glVertex2f(-500, -150);
    glEnd();

    //sun
    glColor3ub(251, 255, 0);
    circle(300, 200, 70);

    //road
    glColor3f(0.6, 0.6, 0.6);
    glRectf(-1000, -200, 1000, -50);


    //Road Lines
    glColor3f(255, 255, 255);
    glRectf(-100, -120, 70, -110);
    glRectf(-360, -120, -190, -110);
    glRectf(180, -120, 370, -110);


    //grass colour
    glColor3f(0.0f, 1.0f, 0.0f);//Green
    glRectf(-1000, -400, 1000, -200);



    //for Cloud
    glColor3ub(220, 232, 229);
    circle(xC + +-123, 151, 26);
    circle(xC + -177, 151, 26);
    circle(xC + -146, 156, 29);
    circle(xC + -145, 146, 29);
    circle(xC + 225, 152, 28);
    circle(xC + 171, 152, 28);
    circle(xC + 198, 155, 29);
    circle(xC + 201, 148, 29);
    circle(xC + -323, 101, 26);
    circle(xC + -377, 101, 26);
    circle(xC + -346, 106, 29);
    circle(xC + -345, 96, 29);

    //Sgin Board
    glColor3ub(90, 90, 90);
    glRectf(-370, 50, -410, -50);
    glColor3ub(0, 35, 255);
    glRectf(-330, 60, -450, 0);
    glColor3ub(255, 255, 255);
    vprint(-440, 28, GLUT_BITMAP_TIMES_ROMAN_24, "ANKARA");




    //name-surname, id
    glColor3f(255, 255, 255);
    glRectf(-530, 320, -220, 240);
    glColor3f(0, 0, 0);
    glRectf(-490, 330, -230, 250);
    glColor3ub(255, 0, 0);
    vprint(-484, 284, GLUT_BITMAP_9_BY_15, " Mustafacan Daþdemir");
    vprint(-474, 266, GLUT_BITMAP_9_BY_15, "    21903303");



}

void display_truck() {

    for (int q = 1; q <= 10; q++)
    {
        xShape = (Shape + q - 1)->sx;
        yShape = (Shape + q - 1)->sy;



        //Lower body
        glBegin(GL_POLYGON);
        glColor3ub(255, 0, 0);
        glVertex2f(xShape + 100, yShape + 20);
        glVertex2f(xShape - 100, yShape + 20);
        glVertex2f(xShape - 100, yShape - 20);
        glVertex2f(xShape + 100, yShape - 20);
        glEnd();

        //Front body
        glBegin(GL_POLYGON);
        glColor3ub(255, 0, 0);
        glVertex2f(xShape + 100, yShape + 20);
        glVertex2f(xShape + 40, yShape + 20);
        glVertex2f(xShape + 40, yShape + 80);
        glVertex2f(xShape + 100, yShape + 80);
        glEnd();

        //Back body
        glBegin(GL_POLYGON);
        glColor3ub(255, 0, 0);
        glVertex2f(xShape + 20, yShape + 20);
        glVertex2f(xShape + 20, yShape + 70);
        glVertex2f(xShape - 100, yShape + 70);
        glVertex2f(xShape - 100, yShape + 20);
        glEnd();



        // lower OUTLINE
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glColor3b(0, 0, 0);
        glVertex2f(xShape + 100, yShape + 20);
        glVertex2f(xShape - 100, yShape + 20);
        glVertex2f(xShape - 100, yShape - 20);
        glVertex2f(xShape + 100, yShape - 20);
        glEnd();

        //Frown body Outline
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glColor3b(0, 0, 0);
        glVertex2f(xShape + 100, yShape + 20);
        glVertex2f(xShape + 40, yShape + 20);
        glVertex2f(xShape + 40, yShape + 80);
        glVertex2f(xShape + 100, yShape + 80);
        glEnd();


        // Window
        glColor3ub(0, 0, 0);
        glBegin(GL_POLYGON);
        glVertex2f(xShape + 100, yShape + 70);
        glVertex2f(xShape + 70, yShape + 70);
        glVertex2f(xShape + 70, yShape + 50);
        glVertex2f(xShape + 100, yShape + 50);
        glEnd();


        //Window Outline
        glBegin(GL_LINE_LOOP);
        glColor3b(90, 90, 90);
        glVertex2f(xShape + 100, yShape + 70);
        glVertex2f(xShape + 70, yShape + 70);
        glVertex2f(xShape + 70, yShape + 50);
        glVertex2f(xShape + 100, yShape + 50);
        glEnd();


        // WHEELS
        glColor3f(0, 0, 0);
        circle(xShape - 20, yShape - 22, 20);
        circle(xShape + 75, yShape - 22, 20);
        circle(xShape - 75, yShape - 22, 20);

        // FRONT LIGHTS
        glColor3ub(255, 255, 255);
        glRectf(xShape + 100, yShape + 13, xShape + 80, yShape + 3);

        // REAR LIGHTS
        glColor3ub(0, 16, 176);
        glRectf(xShape - 100, yShape + 8, xShape - 65, yShape + 3);


    }
}


void menu() {
    glColor3ub(0, 0, 0);
    vprint(250, -240, GLUT_BITMAP_9_BY_15, "F1: Move-Stop");
    vprint(250, -260, GLUT_BITMAP_9_BY_15, "Double click F1: Move again");
}


void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    display_background();
    menu();
    display_truck();

    glColor3ub(0, 0, 102);
    vprint(380, -380, GLUT_BITMAP_9_BY_15, "(%-4d : %-4d)", mx, my);

    glutSwapBuffers();
}

void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}


void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    if (key == GLUT_KEY_F1) {
        if (mode == STOP) {
            mode = ACTION;
            Timer = true;

        }
        else {
            mode = STOP;
            Timer = false;
        }
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}


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
    glutPostRedisplay();
}

//
void onClick(int button, int stat, int x, int y)
{


    xShape = x - winWidth / 2;
    yShape = winHeight / 2 - y;

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && sCnt < 10 && Timer == false) {
        (*(Shape + r)).count = sCnt;
        (*(Shape + r)).sx = xShape;
        (*(Shape + r)).sy = yShape;

        sCnt++;
        r++;

        if (Shape[sCnt].sx > 590) {

            for (int y = 0; y <= 10; y++) {
                Shape[y] = shape_t();//reset the struct
                sCnt = 0;
                r = 0;

            }

        }




    }

    if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN) {
        if (cMode == COLOUR6)
            cMode = COLOUR1;
        else
            cMode++;
    }


    glutPostRedisplay();
}

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

    glutPostRedisplay();
}


void onMove(int x, int y) {

    mx = x - winWidth / 2;
    my = winHeight / 2 - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    xC = xC + 0.3; //clouds speed

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);


    if (Timer)
        for (int q = 0; q <= 10; q++) {
            Shape[q].sx += 5;


        }
    if (Shape[sCnt].sx > 1000) {
        Timer = false;
    }

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
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Trucks");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    // keyboard registration
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    // mouse registration


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
