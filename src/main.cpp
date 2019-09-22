#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "extras.h"
#include <iostream>
#include <time.h>
#include "Bloco.h"
#include <vector>
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
float zdist = 7.0;
float rotationX = -53.0, rotationY = 0.0;
int   last_x = 452;
int last_y = 205;
int   width, height;
bool isOrtho = false;
bool isPaused = false;
bool isFullScreen = false;

///Player
float playerPositionX = 0;
float playerPositonY = -2.90;
float playerSizeX = 3;
float playerSizeY = 0.4;
float playerSizeZ = 0.4;

///Bola
float shooted = false;
float ballSize = 0.09;
float ballSpeedX = 0;
float ballSpeedY = 0;
float ballPositionX = 0;
float ballPositionY = 0;
float forcaBola = 0.02;

///arrow
float arrowSize = 0.9;
float arrowWidth = 0.06;
float arrowAngle = 45;

///Blocos
vector<Bloco*> vetorBlocos;

/// Functions
void preencheVetorBlocos()
{
    float espacamento = 0.2f;
    float tamX = 1.361f;
    float tamY = 0.5f;
    float yBase1  = 2.8;
    float xBase = -4.80f;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<6; j++)
        {
            Ponto p1;
            Ponto p2;
            Ponto p3;
            Ponto p4;

            p1.setX(xBase + espacamento + tamX);
            p1.setY(yBase1 - espacamento);

            p2.setX(xBase + espacamento + tamX);
            p2.setY(yBase1 - espacamento - tamY);

            p3.setX(xBase + espacamento);
            p3.setY(yBase1 - espacamento - tamY);

            p4.setX(xBase + espacamento);
            p4.setY(yBase1 - espacamento);

            Bloco * b1 = new Bloco();

            b1->setP1(p1);
            b1->setP2(p2);
            b1->setP3(p3);
            b1->setP4(p4);

            vetorBlocos.push_back(b1);
            xBase += (espacamento + tamX);
        }
        xBase = -4.80f;
        yBase1 -= (espacamento + tamY);
    }
}

void verificaColisaoBlocos()
{
    float ballX = ballPositionX - ballSize/2;
    float ballY = ballPositionY - ballSize/2;
    int i = 0;
    for (i = 0; i< vetorBlocos.size(); i++)
    {
        if(vetorBlocos[i]->getExibe() && ballX <= vetorBlocos[i]->getP2()->getX() && ballX >= vetorBlocos[i]->getP3()->getX() && ballY >= vetorBlocos[i]->getP3()->getY() && ballY <= vetorBlocos[i]->getP4()->getY())
        {
            vetorBlocos[i]->setExibe(false);
            ballSpeedY = -ballSpeedY;
            /*   if(ballX < vetorBlocos[i]->getP2()->getX() &&  ballX > vetorBlocos[i]->getP3()->getX()) {

                   ballSpeedY = -ballSpeedY;
                   cout<<vetorBlocos[i]->getP2()->getX()<< "!!" << vetorBlocos[i]->getP3()->getX()<< endl;
                   cout<< ballX << endl;
               }
               if(ballY < vetorBlocos[i]->getP3()->getY()) {
                   ballSpeedX = -ballSpeedX;
                   cout<<vetorBlocos[i]->getP3()->getY()<< "!!" << vetorBlocos[i]->getP4()->getY()<< endl;
                   cout<< ballY << endl;
               }*/
        }
    }
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

void verificaGameOver()
{
    if (ballPositionY <= -2.9 )
        shooted = false;
}

void drawBlocos()
{
    for(int i=0; i<vetorBlocos.size(); i++)
    {


        /// Se o bloco puder ser exibido, então exibimos (óbvio)
        if(vetorBlocos[i]->getExibe())
        {

            /// Parede Esquerda ///
            vertice vetorNormal;
            vertice v[8] =
            {
                {vetorBlocos[i]->getP1()->getX(), vetorBlocos[i]->getP1()->getY(),  0.001f},  /// P1
                {vetorBlocos[i]->getP2()->getX(), vetorBlocos[i]->getP2()->getY(),  0.001f},  /// P2
                {vetorBlocos[i]->getP3()->getX(),  vetorBlocos[i]->getP3()->getY(),  0.001f}, /// P3
                {vetorBlocos[i]->getP4()->getX(),  vetorBlocos[i]->getP4()->getY(), 0.001f},   /// P4
                {vetorBlocos[i]->getP1()->getX(), vetorBlocos[i]->getP1()->getY(),  0.5f},  /// P5
                {vetorBlocos[i]->getP2()->getX(), vetorBlocos[i]->getP2()->getY(),  0.5f},  /// P6
                {vetorBlocos[i]->getP3()->getX(),  vetorBlocos[i]->getP3()->getY(),  0.5f}, /// P7
                {vetorBlocos[i]->getP4()->getX(),  vetorBlocos[i]->getP4()->getY(), 0.5f},   /// P8

            };

            triangle t[12] =
            {
                {v[3], v[0], v[1]},
                {v[1], v[2], v[3]},
                {v[7], v[2], v[3]},
                {v[2], v[7], v[6]},
                {v[3], v[0], v[4]},
                {v[3], v[4], v[7]},
                {v[2], v[1], v[5]},
                {v[2], v[5], v[6]},
                {v[1], v[5], v[4]},
                {v[1], v[4], v[0]},
                {v[6], v[7], v[4]},
                {v[6], v[4], v[5]}
            };

            for(int numT = 0; numT < 12; numT++)
            {

                setColor(0,1,1);
                glBegin(GL_TRIANGLES);
                CalculaNormal(t[numT], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
                glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
                for(int j = 0; j < 3; j++) // vertices do triangulo
                    glVertex3d(t[numT].v[j].x, t[numT].v[j].y, t[numT].v[j].z);
                glEnd();
            }
        }
    }
}

void verificaColisaoParede()
{
    if(ballPositionX <= -4.8 || ballPositionX >= 4.8)
    {
        ballSpeedX = -ballSpeedX;
    }
    if(ballPositionY >= 2.8)
    {
        ballSpeedY = -ballSpeedY;
    }
}

void verificaColisaoPlayer ()
{
    //if(ballPositionX <= playerPositionX && ballPositionX >= playerPositionX && ballPositionY <= playerPositonY )
    if(ballPositionX <= playerPositionX + playerSizeX/5 && ballPositionX >= playerPositionX - playerSizeX/5 && ballPositionY <= playerPositonY + playerSizeY/5)
    {
        ballSpeedY = -ballSpeedY;
    }
}

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
    verificaColisaoParede();
    verificaColisaoPlayer();
    glPushMatrix();
    glTranslatef(0, 0, 0.1);
    if(!shooted)
    {
        glTranslatef(playerPositionX, playerPositonY + playerSizeY/2, 0);
    }
    else
    {
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
    glTranslatef(playerPositionX,playerPositonY + playerSizeY/2,0.1);
    glRotatef(arrowAngle,0,0,1);
    glRotatef(-90, 1,0,0);
    setColor(1,0,0);
    glutSolidCone(arrowWidth, arrowSize,100,100);
    glPopMatrix();
}

void drawParedes()
{

    drawBlocos();
    ///Parede Esquerda///
    vertice vetorNormal;

    vertice v[8] =
    {
        {-5.0f, -3.0f,  0.5f}, ///V[0]
        {-4.8f, -3.0f,  0.5f}, ///V[1]
        {-5.0f,  3.0f,  0.5f}, ///V[2]
        {-4.8f,  3.0f, 0.5f}, ///V[3]
        {-4.8f, -3.0f, 0}, ///V[4]
        {-4.8f, 3.0f, 0}, ///V[5]
        {-5.0f,  -3.0f,  0}, ///V[6]
        {-5.0f,  3.0f, 0}, ///V[7]
    };

    triangle t[10] = {{v[0], v[1], v[3]},
        {v[3], v[2], v[0]},
        {v[1], v[4], v[5]},
        {v[5], v[3], v[1]},
        {v[6], v[7], v[2]},
        {v[2], v[0], v[6]},
        {v[6], v[4], v[1]},
        {v[1], v[0], v[6]}
    };

    for(int numT = 0; numT < 8; numT++)
    {

        setColor(0,1,0);
        glBegin(GL_TRIANGLES);
        CalculaNormal(t[numT], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
        glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
        for(int j = 0; j < 3; j++) // vertices do triangulo
            glVertex3d(t[numT].v[j].x, t[numT].v[j].y, t[numT].v[j].z);
        glEnd();
    }

    ///Parede Direita///
    vertice vetorNormalP2;

    vertice v2[8] =
    {
        {5.0f, -3.0f,  0.5f}, ///V[0]
        {4.8f, -3.0f,  0.5f}, ///V[1]
        {5.0f,  3.0f,  0.5f}, ///V[2]
        {4.8f,  3.0f, 0.5f}, ///V[3]
        {4.8f, -3.0f, 0}, ///V[4]
        {4.8f, 3.0f, 0}, ///V[5]
        {5.0f, -3.0f, 0}, ///V[6]
        {5.0f, 3.0f, 0}, ///V[7]
    };

    triangle t2[10] = {{v2[0], v2[1], v2[3]},
        {v2[0], v2[3], v2[2]},
        {v2[4], v2[1], v2[3]},
        {v2[4], v2[3], v2[5]},
        {v2[0], v2[6], v2[7]},
        {v2[7], v2[2], v2[0]},
        {v2[4], v2[6], v2[0]},
        {v2[0], v2[1], v2[4]}
    };

    for(int numT = 0; numT < 8; numT++)
    {

        setColor(0,1,0);
        glBegin(GL_TRIANGLES);
        CalculaNormal(t2[numT], &vetorNormalP2); // Passa face triangular e endereço do vetor normal de saída
        glNormal3f(vetorNormalP2.x, vetorNormalP2.y,vetorNormalP2.z);
        for(int j = 0; j < 3; j++) // vertices do triangulo
            glVertex3d(t2[numT].v[j].x, t2[numT].v[j].y, t2[numT].v[j].z);
        glEnd();
    }

    ///Parede cima

    vertice vetorNormalP3;

    vertice v3[10] =
    {
        {-4.8f, 2.8f, 0},///V[0]
        {4.8f, 2.8f, 0}, ///V[1]
        {4.8f, 2.8f, 0.5f}, ///V[2]
        {-4.8f, 2.8f, 0.5f}, ///V[3]
        {-4.8f, 3.0f, 0.5f}, ///V[4]
        {4.8f, 3.0f, 0.5f}, ///V[5]
        {-5.0f, 3.0f, 0}, ///V[6]
        {5.0f, 3.0f, 0}, ///V[7]
        {-5.0f, 3.0f, 0.5f}, ///V[8]
        {5.0f, 3.0f, 0.5f}, ///V[9]
    };

    triangle t3[10] = {{v3[2], v3[1], v3[0]},
        {v3[0], v3[3], v3[2]},
        {v3[5], v3[2], v3[3]},
        {v3[3], v3[4], v3[5]},
        {v3[6], v3[7], v3[9]},
        {v3[9], v3[8], v3[6]},
    };

    for(int numT = 0; numT < 6; numT++)
    {

        setColor(0,1,0);
        glBegin(GL_TRIANGLES);
        CalculaNormal(t3[numT], &vetorNormalP3); // Passa face triangular e endereço do vetor normal de saída
        glNormal3f(vetorNormalP3.x, vetorNormalP3.y,vetorNormalP3.z);
        for(int j = 0; j < 3; j++) // vertices do triangulo
            glVertex3d(t3[numT].v[j].x, t3[numT].v[j].y, t3[numT].v[j].z);
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

    verificaColisaoBlocos();
    /// Desenha paredes
    drawParedes();

    ///Desenha Player
    drawPlayer();

    ///Desenha Bola
    drawBall();

    ///Desenha Seta
    verificaGameOver();
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

        last_x = x; //452 205
        last_y = y;
        //rotationX = -53;
        //rotationY = 0;
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
        for(int i=0; i<vetorBlocos.size(); i++)
        {
            delete vetorBlocos[i];
        }
        exit(0);
        break;
    case 'p':
        isOrtho = !isOrtho;
        rotationX = 0;
        rotationY = 0;
        last_x = 0;
        last_y = 0;

        reshape(1000, 600);
        break;
    }
}

void specialKey(int key, int x, int y)
{
    if(GLUT_KEY_F12 == key && !isFullScreen)
    {
        glutFullScreen();
        isFullScreen = true;
    }
    else if(GLUT_KEY_F12 == key && isFullScreen)
    {
        glutReshapeWindow(1000, 600);
        glutPositionWindow(0,0);
        isFullScreen = false;
    }
}

// Mouse callback

void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
    }
    /*   if(button == 3) // Scroll up
       {
           zdist+=1.0f;
       }
       if(button == 4) // Scroll Down
       {
           zdist-=1.0f;
       }*/
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
        if(arrowAngle == 0)
        {
            arrowAngle = -5;
        }
    }
    if(button == 4 && arrowAngle < 45) // Scroll Down
    {
        arrowAngle += 5;
        if(arrowAngle == 0)
        {
            arrowAngle = 5;
        }
    }
}




void mousePassive(int x, int y)
{
    if(x > 500 && playerPositionX < 4.18)
    {
        playerPositionX += 0.1;
    }
    else if(x < 500 && playerPositionX > -4.18)
    {
        playerPositionX -= 0.1;
    }
    glutWarpPointer( 500, 300 );
}

/// Main
int main(int argc, char** argv)
{
    preencheVetorBlocos();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1000, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    //glutFullScreen();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mousePassive);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutMainLoop();
    return 0;
}
