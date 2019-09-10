#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "extras.h"
#include <iostream>
#include <time.h>
//#include <player.h>

using namespace std;

/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int   width, height;
bool isOrtho = false;

///Player
float playerPositionX = 0;
float playerPositonY = -2;
float playerSizeX = 1;
float playerSizeY = 0.4;
float playerSizeZ = 0.4;

/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
}

void drawPlayer()
{
    glPushMatrix();
    setColor(0,0,1);
    glTranslatef(playerPositionX, playerPositonY, 0);
    glScalef(playerSizeX, playerSizeY, playerSizeZ);
    glutSolidCube(0.4);
    glPopMatrix();
}

void drawObject()
{
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (0.0, 0.0, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef( rotationY, 0.0, 1.0, 0.0 );
    glRotatef( rotationX, 1.0, 0.0, 0.0 );
    drawObject();
    glPopMatrix();

    drawPlayer();

    glutSwapBuffers();
}

void idle ()
{
    glutPostRedisplay();
}

void reshape (int w, int h)
{
    width = w;
    height = h;

    if(isOrtho)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho( -4, 4, -4, 4, 0.01, 200.0);
    }
    else
    {
        glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 0.01, 200.0);
    }

}

void keyboard (unsigned char key, int x, int y)
{

    switch (tolower(key))
    {
    case 27:
        exit(0);
        break;
    case 'p':
        isOrtho = !isOrtho;
        reshape(1000, 600);
    }
}

// Motion callback
void motion(int x, int y )
{
    rotationX += (float) (y - last_y);
    rotationY += (float) (x - last_x);

    last_x = x;
    last_y = y;
}

// Mouse callback

void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
    }
    if(button == 3) // Scroll up
    {
        zdist+=1.0f;
    }
    if(button == 4) // Scroll Down
    {
        zdist-=1.0f;
    }
}

void mousePassive(int x, int y)
{
    double posiAux = 0;

    if(x == 500)
    {
        posiAux = 0;
    }
    else if(x > 500) {
        posiAux = (double)(x-500)/125;
    }
    else if(x < 500){
        posiAux = (double)(x-500)/125;
    }
    playerPositionX = (posiAux+(playerSizeX/2));
    cout<< posiAux-(playerSizeX/2) <<endl;
}

/// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1000, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mousePassive);
    glutMainLoop();

    return 0;
}
