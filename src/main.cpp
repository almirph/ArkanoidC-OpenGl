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

///Classes
class vertice
{
public:
    float x,y,z;
};

class triangle
{
public:
    vertice v[3];
};


/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int   width, height;
bool isOrtho = false;
bool isPaused = false;

///Player
float playerPositionX = 0;
float playerPositonY = -2;
float playerSizeX = 1;
float playerSizeY = 0.4;
float playerSizeZ = 0.4;

///Bola
float shooted = false;
float ballSize = 0.09;
float ballSpeedX = 0;
float ballSpeedY = 0;
float ballPositionX = 0;
float ballPositionY = 0;
float forcaBola = 0.01;

///arrow
float arrowSize = 0.09;
float arrowAngle = 0;

/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
}

void calculaVelocidadeBola(int angulo)
{
    if(angulo < 0)
    {
        ballSpeedX = fabs(sin(angulo) * forcaBola);
    }
    else if(angulo > 0)
    {
        ballSpeedX = -fabs(sin(angulo) * forcaBola);
    }
    if (angulo == 0)
    {
        ballSpeedY = forcaBola;
        ballSpeedX = 0;
    }
    else
    {
        ballSpeedY = fabs(cos(angulo) * forcaBola);
    }
}

void drawPlayer()
{
    glPushMatrix();
    setColor(0,0,1);
    glTranslatef(playerPositionX, playerPositonY, 0.1);
    glScalef(playerSizeX, playerSizeY, playerSizeZ);
    glutSolidCube(0.4);
    glPopMatrix();
}

void drawBall()
{
    glPushMatrix();
        glTranslatef(0, 0, 0.1);
        if(!shooted){
            glTranslatef(playerPositionX, playerPositonY + playerSizeY/2 , 0);
        }
        else {
            ballPositionX += ballSpeedX;
            ballPositionY += ballSpeedY;
            glTranslatef(ballPositionX, ballPositionY, 0);
        }
        setColor(1,0,0);
        glutSolidSphere(ballSize,100,100);
    glPopMatrix();
}

void drawSeta()
{
    glPushMatrix();
    glTranslatef(playerPositionX,playerPositonY + playerSizeY,0.1);
    glRotatef(arrowAngle,0,0,1);
    glRotatef(-90, 1,0,0);
    setColor(1,0,0);
    glutSolidCone(0.06, 0.9,100,100);
    glPopMatrix();
}

void CalculaNormal(triangle t, vertice *vn)
{
    vertice v_0 = t.v[0],
            v_1 = t.v[1],
            v_2 = t.v[2];
    vertice v1, v2;
    double len;

    /* Encontra vetor v1 */
    v1.x = v_1.x - v_0.x;
    v1.y = v_1.y - v_0.y;
    v1.z = v_1.z - v_0.z;

    /* Encontra vetor v2 */
    v2.x = v_2.x - v_0.x;
    v2.y = v_2.y - v_0.y;
    v2.z = v_2.z - v_0.z;

    /* Calculo do produto vetorial de v1 e v2 */
    vn->x = (v1.y * v2.z) - (v1.z  * v2.y);
    vn->y = (v1.z * v2.x) - (v1.x * v2.z);
    vn->z = (v1.x * v2.y) - (v1.y * v2.x);

    /* normalizacao de n */
    len = sqrt(pow(vn->x,2) + pow(vn->y,2) + pow(vn->z,2));

    vn->x /= len;
    vn->y /= len;
    vn->z /= len;
}

void drawParedes()
{
    vertice vetorNormal;

    vertice v[8] =
    {
        {-5.0f, -3.0f,  0.5f},
        {-4.5f, -3.0f,  0.5f},
        {-5.0f,  3.0f,  0.5f},
        {-4.50f,  3.0f, 0.5f},
        {-4.5f, -3.0f, 0},
        {-4.5f, 3.0f, 0}
    };

    triangle t[10] = {{v[0], v[1], v[3]},
        {v[0], v[3], v[2]},
        {v[4], v[1], v[2]},
        {v[4], v[3], v[5]}
    };

    for(int numT = 0; numT < 4; numT++)
    {

        setColor(0,0,0);
        glBegin(GL_TRIANGLES);
        CalculaNormal(t[numT], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
        glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
        for(int j = 0; j < 3; j++) // vertices do triangulo
            glVertex3d(t[numT].v[j].x, t[numT].v[j].y, t[numT].v[j].z);
        glEnd();
    }
}

void drawObject()
{
    vertice vetorNormal;

    vertice v[8] =
    {
        {-5.0f, -3.0f,  0.0f},
        {-5.0f, 3.0f,  0.0f},
        {5.0f,  3.0f,  0.0f},
        {5.0f,  -3.0f, 0.0f}
    };


    triangle t[10] = {{v[0], v[1], v[2]},
        {v[0], v[3], v[2]},
    };

    for(int numT = 0; numT < 2; numT++)
    {

        setColor(0,0,139);
        glBegin(GL_TRIANGLES);
        CalculaNormal(t[numT], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
        glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
        for(int j = 0; j < 3; j++) // vertices do triangulo
            glVertex3d(t[numT].v[j].x, t[numT].v[j].y, t[numT].v[j].z);
        glEnd();
    }

    /// Desenha paredes
    //drawParedes();

    ///Desenha Player
    drawPlayer();

    ///Desenha Bola
    drawBall();

    ///Desenha Seta
    if(!shooted)
    {
        drawSeta();
    }
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

    glutSwapBuffers();
}

void idle ()
{
    glutPostRedisplay();
}

// Motion callback
void motion(int x, int y )
{
    if(!isOrtho)
    {
        rotationX += (float) (y - last_y);
        rotationY += (float) (x - last_x);

        last_x = x;
        last_y = y;
    }
    else
    {
        rotationX = 0;
        rotationY = 0;

        last_x = 0;
        last_y = 0;

    }
}

//reshape
void reshape (int w, int h)
{
    width = w;
    height = h;

    if(isOrtho)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho( -5, 5, -3, 3, 0.01, 200.0);
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
        rotationX = 0;
        rotationY = 0;
        last_x = 0;
        last_y = 0;

        reshape(1000, 600);
    }
}

// Mouse callback

void mouse(int button, int state, int x, int y)
{
    if(isPaused)
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
    if ( button == GLUT_LEFT_BUTTON && !shooted)
    {
        ballPositionY = playerPositonY + playerSizeY/2;
        ballPositionX = playerPositionX;
        calculaVelocidadeBola(arrowAngle);
        shooted = true;
    }
    if(button == 3 && arrowAngle > -45) // Scroll up
    {
        arrowAngle -= 5;
    }
    if(button == 4 && arrowAngle < 45) // Scroll Down
    {
        arrowAngle += 5;
    }
}




void mousePassive(int x, int y)
{
    double posiAux = 0;

    if(x == 500)
    {
        posiAux = 0;
    }
    else if(x > 500)
    {
        posiAux = (double)(x-500)/125;
        playerPositionX = posiAux + playerSizeX/2;
    }
    else if(x < 500)
    {
        posiAux = (double)(x-500)/125;
        playerPositionX = posiAux - playerSizeX/2;
    }
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
