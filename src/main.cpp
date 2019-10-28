#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include <iostream>
#include <time.h>
#include "Bloco.h"
#include <vector>

#define PI 3.14159265359

#include "glcWavefrontObject.h"

#define NUM_OBJECTS 2

using namespace std;


char objectFiles[NUM_OBJECTS][50] =
{
    "../data/obj/ball.obj",
    "../data/obj/al.obj",
};

typedef struct
{
    //GLMmodel* pmodel = NULL;
    glcWavefrontObject *pmodel = NULL;
} object;

object *objectList;

glcWavefrontObject *objectManager = NULL;

float obj1X = 2;
float obj1Y = 2.5;
int selected = 0;
int selectedShading = SMOOTH_SHADING;
int selectedRender = USE_MATERIAL;

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
int qntVidas = 5;
bool isOrtho = false;
bool isFullScreen = false;
bool isEndGame = false;
float raioCurvaParede = 0.5;
float raioPlayer = 0.28;

///Player
float playerPositionX = 0;
float playerPositonY = -3.65;

///Bola
float shooted = false;
float ballSize = 0.09;
float ballSpeedX = 0;
float ballSpeedY = 0;
float ballPositionX = 0;
float ballPositionY = 0;
float forcaBola = 0.03;

///arrow
float arrowSize = 0.9;
float arrowWidth = 0.06;
float arrowAngle = -10;

/// Pause
bool jogoPausado = false;
bool podeMovimentar = false;

///Blocos
vector<Bloco*> vetorBlocos;

///Pontos paredes curva - Direita
vector<Ponto*> vetorPontosParedeDireita;

int faseAtual = 1;

/// Functions
void preencheVetorBlocos()
{
    if(faseAtual == 1)
    {
        float espacamento = 0.4f;
        float tamX = 2.55f;
        float tamY = 1;
        float yBase1  = 2.8;
        float xBase = -4.80f;

        for(int i=0; i<2; i++)
        {
            for(int j=0; j<3; j++)
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
    else if(faseAtual == 2)
    {
        float espacamento = 0.2f;
        float tamX = 1.361f;
        float tamY = 0.5f;
        float yBase1  = 2.8;
        float xBase = -4.80f;

        for(int i=0; i<2; i++)
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
    else if(faseAtual == 3)
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
}

void setMaterial(float brilho, float ambiente[], float difusa[], float especular[])
{
    GLfloat object_emissao[] =   { 0.0, 0.0, 0.0, 0.0 };
    // Material do objeto (neste caso, ruby). Parametros em RGBA
    GLfloat objeto_ambient[]   = { ambiente[0], ambiente[1], ambiente[2], 1.0 };
    GLfloat objeto_difusa[]    = { difusa[0], difusa[1], difusa[2], 1.0 };
    GLfloat objeto_especular[] = { especular[0], especular[1], especular[2], 1.0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, object_emissao);
    GLfloat objeto_brilho[]    = { brilho };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

void verificaColisaoBlocos()
{
    float ballX = ballPositionX + ballSize/2;
    float ballY = ballPositionY + ballSize/2;
    int i = 0;
    for (i = 0; i< vetorBlocos.size(); i++)
    {
        if(vetorBlocos[i]->getExibe() && vetorBlocos[i]->getP3()->getX() <= ballX && vetorBlocos[i]->getP2()->getX() >= ballX  && vetorBlocos[i]->getP2()->getY() >= ballY && vetorBlocos[i]->getP2()->getY() - 0.05 <= ballY)
        {
            vetorBlocos[i]->setExibe(false);
            ballSpeedY = -ballSpeedY;
        }
        if(vetorBlocos[i]->getExibe() && vetorBlocos[i]->getP4()->getX() <= ballX && vetorBlocos[i]->getP1()->getX() >= ballX  && vetorBlocos[i]->getP4()->getY() >= ballY && vetorBlocos[i]->getP4()->getY() - 0.05 <= ballY)
        {
            vetorBlocos[i]->setExibe(false);
            ballSpeedY = -ballSpeedY;
        }
        if(vetorBlocos[i]->getExibe() && vetorBlocos[i]->getP3()->getY() <= ballY && vetorBlocos[i]->getP4()->getY() >= ballY  && vetorBlocos[i]->getP4()->getX() >= ballX && vetorBlocos[i]->getP2()->getX() - 0.05 <= ballX)
        {
            vetorBlocos[i]->setExibe(false);
            ballSpeedX = -ballSpeedX;
        }
        if(vetorBlocos[i]->getExibe() && vetorBlocos[i]->getP2()->getY() <= ballY && vetorBlocos[i]->getP1()->getY() >= ballY  && vetorBlocos[i]->getP2()->getX() >= ballX && vetorBlocos[i]->getP4()->getX() - 0.05 <= ballX)
        {
            vetorBlocos[i]->setExibe(false);
            ballSpeedX = -ballSpeedX;
        }
    }
}

void verificaColisaoParedesPlanas()
{
    float ballX = ballPositionX;
    float ballY = ballPositionY;

    /// Colidiu com a parede à direita
    if((ballX + ballSize) >= 4.8)
    {
        ballSpeedX = -ballSpeedX;
    }

    /// Colidiu com a parede à esquerda
    if((ballX - ballSize) <= -4.8)
    {
        ballSpeedX = -ballSpeedX;
    }

    /// Colide parede superior
    if((ballY + ballSize) >= (3-ballSize))
    {
        ballSpeedY = -ballSpeedY;
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

void reiniciaJogo()
{
    shooted = false;
    int i = 0;
    for (i = 0; i< vetorBlocos.size(); i++)
    {
        vetorBlocos[i]->setExibe(true);
    }

    qntVidas = 5;
    faseAtual = 1;
}

void verificaGameOver()
{
    if (ballPositionY <= -2.9 && shooted)
    {
        if((qntVidas-1) >= 0)
        {
            qntVidas--;
        }

        shooted = false;
        if(qntVidas == 0)
        {
            faseAtual = 1;
            reiniciaJogo();
        }
    }
}

bool verificaEndGame()
{
    int i = 0;
    for (i = 0; i< vetorBlocos.size(); i++)
    {
        if(vetorBlocos[i]->getExibe())
        {
            return false;
        }
    }

    if((faseAtual+1) <= 3 && shooted)
    {
        faseAtual++;
        shooted = false;

        for(int i=0; i < vetorBlocos.size(); i++)
        {
            delete vetorBlocos[i];
        }

        for(int i=0; i<vetorBlocos.size(); i++)
        {
            vetorBlocos.pop_back();
        }

        preencheVetorBlocos();

        return false;
    }

    return true;
}

void drawBlocos()
{
    float ambient[3]  = {0.0, 0.355, 0.0};
    float difusa[3] = {0.0, 0.4, 0.0};
    float especular[3] = {0.0, 1, 0.0};

    if(faseAtual == 1)
    {
        ambient[0] = 0.5;
        ambient[1] = 0.1;
        ambient[2] = 0.4;

        difusa[0] = 0.5;
        difusa[1] = 0.1;
        difusa[2] = 0.4;

        especular[0] = 0.5;
        especular[1] = 0.1;
        especular[2] = 0.4;
    }
    else if(faseAtual == 2)
    {
        ambient[0] = 0.18;
        ambient[1] = 0.23;
        ambient[2] = 0.55;

        difusa[0] = 0.18;
        difusa[1] = 0.23;
        difusa[2] = 0.55;

        especular[0] = 0.18;
        especular[1] = 0.23;
        especular[2] = 0.55;
    }
    else
    {
        ambient[0] = 0.651;
        ambient[1] = 0.565;
        ambient[2] = 0.651;

        difusa[0] = 0.651;
        difusa[1] = 0.565;
        difusa[2] = 0.651;

        especular[0] = 0.651;
        especular[1] = 0.565;
        especular[2] = 0.651;
    }

    float brilho = 20.0;
    for(int i=0; i<vetorBlocos.size(); i++)
    {

        /// Se o bloco puder ser exibido, então exibimos (óbvio)
        if(vetorBlocos[i]->getExibe())
        {

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

                setMaterial(brilho, ambient, difusa,especular );
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


void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);               // Habilita Z-buffer
    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_LIGHT0);                   // habilita luz 0

    // Cor da fonte de luz (RGBA)
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
    // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
    GLfloat posicao_luz[] = { 0, -5, 1.0, 1.0};

    // Define parametros da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    // LOAD OBJECTS
    objectManager = new glcWavefrontObject();
    objectManager->SetNumberOfObjects(NUM_OBJECTS);
    for(int i = 0; i < NUM_OBJECTS; i++)
    {

        objectManager->SelectObject(i);
        objectManager->ReadObject(objectFiles[i]);
        objectManager->Unitize();
        objectManager->FacetNormal();
        objectManager->VertexNormals(90.0);
        objectManager->Draw();
    }
}

void calculaVelocidadeBola(int angulo)
{
    double anguloRadianos = ((angulo * PI)/180);

    ballSpeedX = (angulo >= 0) ? -fabs(sin(anguloRadianos) * forcaBola) : fabs(sin(anguloRadianos) * forcaBola);
    ballSpeedY = fabs(cos(anguloRadianos) * forcaBola);

    cout<<"ballSpeedX: "<<ballSpeedX<<endl;
    cout<<"ballSpeedY: "<<ballSpeedY<<endl;
}


void drawBall()
{
    float ambient[3]   = {0.255, 0, 0};
    float difusa[3] = {0.255, 0, 0.};
    float especular[3] = {0.255, 0, 0};
    float brilho = 90.0;
    if(!isEndGame)
    {
        glPushMatrix();
        glTranslatef(0, 0, 0.1);
        if(!shooted)
        {
            glTranslatef(playerPositionX, playerPositonY + 1, 0);
        }
        else
        {
            if(!jogoPausado)
            {
                ballPositionX += ballSpeedX;
                ballPositionY += ballSpeedY;
            }
            glTranslatef(ballPositionX, ballPositionY, 0);
        }
        setMaterial(brilho, ambient, difusa,especular );
        glutSolidSphere(ballSize,100,100);
        glPopMatrix();
    }

}

void drawSeta()
{
    float ambient[3]   = {0.255, 0, 0};
    float difusa[3] = {0.255, 0, 0.};
    float especular[3] = {0.255, 0, 0};
    float brilho = 90.0;
    if(!isEndGame)
    {
        glPushMatrix();
        glTranslatef(playerPositionX,playerPositonY+1,0.1);
        glRotatef(arrowAngle,0,0,1);
        glRotatef(-90, 1,0,0);
        setMaterial(brilho, ambient, difusa,especular );
        glutSolidCone(arrowWidth, arrowSize,100,100);
        glPopMatrix();
    }
}

void desenharVidas()
{
    float ambient[3]   = {0.255, 0, 0};
    float difusa[3] = {0.255, 0, 0.};
    float especular[3] = {0.255, 0, 0};
    float brilho = 90.0;
    float auxPos = 0.1;
    for(int i=0; i<qntVidas; i++)
    {

        if(isOrtho)
        {
            glPushMatrix();
            glTranslatef(auxPos -5, 2.9, 0);
            setMaterial(brilho, ambient, difusa,especular );
            glutSolidSphere(ballSize/1.1,100,100);
            glPopMatrix();
            auxPos += 0.3;
        }
        else
        {
            glPushMatrix();
            glTranslatef(auxPos -5, 2.9, 1.6);
            //setColor(1,0,0);
            glutSolidSphere(ballSize/1.1,100,100);
            glPopMatrix();
            auxPos += 0.3;
        }
    }
}

void drawParedes()
{
    float ambient[3]   = {0.255, 0, 0.255};
    float difusa[3] = {0.255, 0, 0.255};
    float especular[3] = {0.255, 0, 0.255};
    float brilho = 90.0;

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

    triangle t[10] = {{v[1], v[0], v[2]},
        {v[2], v[3], v[1]},
        {v[6], v[4], v[0]},
        {v[4], v[1], v[0]},
        {v[4], v[5], v[1]},
        {v[5], v[3], v[1]},
        {v[6], v[7], v[0]},
        {v[7], v[2], v[0]}
    };

    for(int numT = 0; numT < 8; numT++)
    {

        if(!isEndGame)
            setMaterial(brilho, ambient, difusa,especular );
        else
            setMaterial(brilho, ambient, difusa,especular );
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
        if(!isEndGame)
            setMaterial(brilho, ambient, difusa,especular );
        else
            setMaterial(brilho, ambient, difusa,especular );
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
        if(!isEndGame)
            setMaterial(brilho, ambient, difusa,especular );
        else
            setMaterial(brilho, ambient, difusa,especular );
        glBegin(GL_TRIANGLES);
        CalculaNormal(t3[numT], &vetorNormalP3); // Passa face triangular e endereço do vetor normal de saída
        glNormal3f(vetorNormalP3.x, vetorNormalP3.y,vetorNormalP3.z);
        for(int j = 0; j < 3; j++) // vertices do triangulo
            glVertex3d(t3[numT].v[j].x, t3[numT].v[j].y, t3[numT].v[j].z);
        glEnd();
    }
}



void desenharPause()
{
    float ambient[3]   = {0.255, 0, 0};
    float difusa[3] = {0.255, 0, 0.};
    float especular[3] = {0.255, 0, 0};
    float brilho = 90.0;
    if(jogoPausado)
    {
        ///Parede cima

        vertice vetorNormalP3;

        vertice v3[8] =
        {
            {-0.25f, 0.4f, 0.05f},
            {-0.25f, -0.4f, 0.05f},
            {0.05f, 0.4f, 0.05f},
            {0.05f, -0.4f, 0.05f},

            {0.25f, 0.4f, 0.05f},
            {0.25f, -0.4f, 0.05f},
            {0.55f, 0.4f, 0.05f},
            {0.55f, -0.4f, 0.05f}
        };

        triangle t3[4] =
        {
            {v3[1], v3[3], v3[2]},
            {v3[2], v3[0], v3[1]},
            {v3[4], v3[5], v3[7]},
            {v3[7], v3[6], v3[4]}
        };

        for(int numT = 0; numT < 4; numT++)
        {

            setMaterial(brilho, ambient, difusa,especular );
            glBegin(GL_TRIANGLES);
            CalculaNormal(t3[numT], &vetorNormalP3); // Passa face triangular e endereço do vetor normal de saída
            glNormal3f(vetorNormalP3.x, vetorNormalP3.y,vetorNormalP3.z);
            for(int j = 0; j < 3; j++) // vertices do triangulo
                glVertex3d(t3[numT].v[j].x, t3[numT].v[j].y, t3[numT].v[j].z);
            glEnd();
        }
    }
}

void drawCurvaDireita(int anguloInicio, int anguloFinal)
{

    float ambient[3]   = {0.255, 0, 0.255};
    float difusa[3] = {0.255, 0, 0.255};
    float especular[3] = {0.255, 0, 0.255};
    float brilho = 90.0;

    vertice vetorNormal;
    float x = 0;
    float y = 0;
    float xAnterior = 0;
    float yAnterior = 0;
    float cosAngulo;
    float senAngulo;
    for(int i = anguloInicio; i < anguloFinal; i += 1)
    {
        cosAngulo = cos(((i * PI)/180));
        senAngulo = sin(((i * PI)/180));

        x = (raioCurvaParede * PI  * cosAngulo) + 6;
        y = raioCurvaParede * PI * senAngulo;

        if(xAnterior != 0 && yAnterior != 0)
        {
            vertice v[5] = {{xAnterior, yAnterior, 0}, {x, y, 0}, {x, y, 0.5}, {xAnterior, yAnterior, 0.5}, {4.8, 0, 0.5}};
            setMaterial(brilho, ambient, difusa,especular );
            glBegin(GL_TRIANGLES);
            triangle t[4] = {{v[0], v[1], v[3]}, {v[1], v[2], v[3]}, {v[4], v[2], v[3]}};
            for(int k = 0; k < 3; k++)
            {
                CalculaNormal(t[k], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
                glNormal3f(vetorNormal.x, vetorNormal.y, vetorNormal.z);
                for(int j = 0; j < 3; j++) // vertices do triangulo
                {
                    glVertex3d(t[k].v[j].x, t[k].v[j].y, t[k].v[j].z);
                }
            }

            glEnd();

            float paredeX = x;
            float paredeY = y;

            if((ballPositionX + ballSize) > paredeX && ballPositionY >= (paredeY-0.008) && ballPositionY <= (paredeY+0.008))
            {
                CalculaNormal(t[0], &vetorNormal);
                cout<<"vetorNormal.x: "<<vetorNormal.x<<endl;
                cout<<"vetorNormal.y: "<<vetorNormal.y<<endl;

                ballSpeedX = ((vetorNormal.x + ballSpeedX)/sqrt(pow(vetorNormal.x + ballSpeedX,2) + pow(vetorNormal.y + ballSpeedY, 2))) * forcaBola;
                ballSpeedY = ((vetorNormal.y + ballSpeedY)/sqrt(pow(vetorNormal.x + ballSpeedX,2) + pow(vetorNormal.y + ballSpeedY, 2))) * forcaBola;
            }
        }

        xAnterior = x;
        yAnterior = y;
    }
}

void drawCurvaEsquerda(int anguloInicio, int anguloFinal)
{
    float ambient[3]   = {0.255, 0, 0.255};
    float difusa[3] = {0.255, 0, 0.255};
    float especular[3] = {0.255, 0, 0.255};
    float brilho = 90.0;

    vertice vetorNormal;
    float x = 0;
    float y = 0;
    float xAnterior = 0;
    float yAnterior = 0;
    float cosAngulo;
    float senAngulo;
    for(int i = anguloInicio; i < anguloFinal; i += 1)
    {
        cosAngulo = cos(((i * PI)/180));
        senAngulo = sin(((i * PI)/180));

        x = (raioCurvaParede * PI  * cosAngulo) - 6;
        y = raioCurvaParede * PI * senAngulo;

        if(xAnterior != 0 && yAnterior != 0)
        {
            vertice v[5] = {{xAnterior, yAnterior, 0}, {x, y, 0}, {x, y, 0.5}, {xAnterior, yAnterior, 0.5}, {-4.8, 0, 0.5}};
            setMaterial(brilho, ambient, difusa,especular );
            glBegin(GL_TRIANGLES);
            triangle t[4] = {{v[0], v[1], v[3]}, {v[1], v[2], v[3]}, {v[4], v[2], v[3]}};
            for(int k = 0; k < 3; k++)
            {
                CalculaNormal(t[k], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
                glNormal3f(vetorNormal.x, vetorNormal.y, vetorNormal.z);
                for(int j = 0; j < 3; j++) // vertices do triangulo
                {
                    glVertex3d(t[k].v[j].x, t[k].v[j].y, t[k].v[j].z);
                }
            }
            glEnd();

            float paredeX = x;
            float paredeY = y;

            if((ballPositionX - ballSize) < paredeX && ballPositionY >= (paredeY-0.008) && ballPositionY <= (paredeY+0.008))
            {
                CalculaNormal(t[0], &vetorNormal);
                cout<<"vetorNormal.x: "<<vetorNormal.x<<endl;
                cout<<"vetorNormal.y: "<<vetorNormal.y<<endl;

                ballSpeedX = ((vetorNormal.x + ballSpeedX)/sqrt(pow(vetorNormal.x + ballSpeedX,2) + pow(vetorNormal.y + ballSpeedY, 2))) * forcaBola;
                ballSpeedY = ((vetorNormal.y + ballSpeedY)/sqrt(pow(vetorNormal.x + ballSpeedX,2) + pow(vetorNormal.y + ballSpeedY, 2))) * forcaBola;
            }
        }

        xAnterior = x;
        yAnterior = y;
    }
}

drawPlayer()
{
    float ambient[3]   = {.255, 0, 0};
    float difusa[3] = {.255, 0, 0};
    float especular[3] = {.255, 0, 0};
    float brilho = 90.0;
    int anguloInicio = 45;
    int anguloFinal = 135;
    vertice vetorNormal;
    float x = 0;
    float y = 0;
    float xAnterior = 0;
    float yAnterior = 0;
    float cosAngulo;
    float senAngulo;
    for(int i = anguloInicio; i < anguloFinal; i += 1)
    {
        cosAngulo = cos(((i * PI)/180));
        senAngulo = sin(((i * PI)/180));
        x = (raioPlayer * PI  * cosAngulo) + playerPositionX;
        y = (raioPlayer * PI * senAngulo) + playerPositonY;
        if(xAnterior != 0 && yAnterior != 0)
        {
            vertice v[5] = {{xAnterior, yAnterior, 0}, {x, y, 0}, {x, y, 0.5}, {xAnterior, yAnterior, 0.5}, {playerPositionX, -3, 0.5}, };
            setMaterial(brilho, ambient, difusa,especular );
            glBegin(GL_TRIANGLES);
            triangle t[4] = {{v[0], v[1], v[3]}, {v[1], v[2], v[3]}, {v[4], v[2], v[3]}};
            for(int k = 0; k < 3; k++)
            {
                CalculaNormal(t[k], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
                glNormal3f(vetorNormal.x, vetorNormal.y, vetorNormal.z);
                for(int j = 0; j < 3; j++) // vertices do triangulo
                {
                    glVertex3d(t[k].v[j].x, t[k].v[j].y, t[k].v[j].z);
                }
            }
            glEnd();

            float paredeX = x;
            float paredeY = y;

            if((ballPositionY - ballSize) < paredeY && ballPositionX >= (paredeX-0.008) && ballPositionX <= (paredeX+0.008))
            {
                CalculaNormal(t[0], &vetorNormal);
                cout<<"vetorNormal.x: "<<vetorNormal.x<<endl;
                cout<<"vetorNormal.y: "<<vetorNormal.y<<endl;

                ballSpeedX = ((vetorNormal.x + ballSpeedX)/sqrt(pow(vetorNormal.x + ballSpeedX,2) + pow(vetorNormal.y + ballSpeedY, 2))) * forcaBola;
                ballSpeedY = ((vetorNormal.y + ballSpeedY)/sqrt(pow(vetorNormal.x + ballSpeedX,2) + pow(vetorNormal.y + ballSpeedY, 2))) * forcaBola;
            }
        }

        xAnterior = x;
        yAnterior = y;
    }

    ///Desenha parte de trás do jogador
    float xInicial = ((cos((anguloInicio * PI)/180)) * raioPlayer * PI) + playerPositionX;
    float xFinal = ((cos((anguloFinal * PI)/180)) * raioPlayer * PI) + playerPositionX;

    vertice vetorNormalP;

    vertice v2[4] =
    {
        {xInicial, -3.02f,  0}, ///V[0]
        {xFinal, -3.02f,  0}, ///V[1]
        {xFinal,  -3.02f,  0.5f}, ///V[2]
        {xInicial,  -3.02f, 0.5f}, ///V[3]
    };

    triangle t2[2] = {{v2[1], v2[0], v2[3]},
        {v2[3], v2[2], v2[1]},
    };

    for(int numT = 0; numT < 2; numT++)
    {
        setMaterial(brilho, ambient, difusa,especular );
        glBegin(GL_TRIANGLES);
        CalculaNormal(t2[numT], &vetorNormalP); // Passa face triangular e endereço do vetor normal de saída
        glNormal3f(vetorNormalP.x, vetorNormalP.y,vetorNormalP.z);
        for(int j = 0; j < 3; j++) // vertices do triangulo
            glVertex3d(t2[numT].v[j].x, t2[numT].v[j].y, t2[numT].v[j].z);
        glEnd();
    }

}

void drawObject()
{
    float ambient[3]   = {0.1, 0, 0.7};
    float difusa[3] = {0, 0, 0.1};
    float especular[3] = {0, 0.1, 1};
    float brilho = 90.0;
    vertice vetorNormal;

    vertice v[8] =
    {
        {-5.0f, -3.0f,  0.0f},
        {-5.0f, 3.0f,  0.0f},
        {5.0f,  3.0f,  0.0f},
        {5.0f,  -3.0f, 0.0f}
    };


    triangle t[10] = {{v[1], v[0], v[3]},
        {v[3], v[2], v[1]},
    };

    for(int numT = 0; numT < 2; numT++)
    {
        if(!isEndGame)
            setMaterial(brilho, ambient, difusa,especular );
        else
            setMaterial(brilho, ambient, difusa,especular );
        glBegin(GL_TRIANGLES);
        CalculaNormal(t[numT], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
        glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
        for(int j = 0; j < 3; j++) // vertices do triangulo
            glVertex3d(t[numT].v[j].x, t[numT].v[j].y, t[numT].v[j].z);
        glEnd();
    }

    isEndGame = verificaEndGame();
    verificaColisaoBlocos();
    verificaColisaoParedesPlanas();

    ///Desenha Jogador
    drawPlayer();

    ///Desenha paredes
    drawCurvaDireita(135, 225);
    drawCurvaEsquerda(315, 380);
    drawCurvaEsquerda(0, 45);
    drawParedes();

    ///Desenha Blcos
    drawBlocos();

    ///Desenha Bola
    drawBall();

    /// Desenha pause
    desenharPause();

    ///Desenha Seta
    verificaGameOver();
    if(!shooted)
    {
        drawSeta();
    }
}

void display(void)
{
    if(!jogoPausado)
    {
        glutWarpPointer( 500, 300 );
    }

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (0.0, 0.0, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef( rotationY, 0.0, 1.0, 0.0 );
    glRotatef( rotationX, 1.0, 0.0, 0.0 );
    drawObject();
    glPopMatrix();

    ///Object manager
    glPushMatrix();
    objectManager->SelectObject(selected);
    objectManager->SetShadingMode(selectedShading); // Possible values: FLAT_SHADING e SMOOTH_SHADING
    objectManager->SetRenderMode(selectedRender);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
    objectManager->Unitize();
    objectManager->Scale(0.3);
    glTranslatef(obj1X, obj1Y,-3);
    objectManager->Draw();
    glPopMatrix();

    ///Desenha vidas
    desenharVidas();

    glutSwapBuffers();
}

void idle ()
{
    glutPostRedisplay();
}




// Motion callback
void motion(int x, int y )
{
    if(!isOrtho && (jogoPausado && podeMovimentar))
    {
        rotationX += (float) (y - last_y);
        rotationY += (float) (x - last_x);

        last_x = x; //452 205
        last_y = y;

        GLfloat posicao_luz[] = { rotationX, rotationY, 1.0, 1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
    }
    else if(isOrtho)
    {
        rotationX = 0;
        rotationY = 0;

        last_x = 0;
        last_y = 0;

        GLfloat posicao_luz[] = { 0, 0, 6.0, 1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
    }
}

//reshape
void reshape (int w, int h)
{
    width = w;
    height = h;

    if(isOrtho)
    {
        glViewport (0, 0, (GLsizei) w, (GLsizei) h);
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

        for(int i=0; i< vetorPontosParedeDireita.size(); i++)
        {
            delete vetorPontosParedeDireita[i];
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
    case ' ':
        jogoPausado = !jogoPausado;

        if(!jogoPausado)
        {
            podeMovimentar = false;
        }

        break;
    case 'r':
        reiniciaJogo();
        break;
    case 'c':

        podeMovimentar = !podeMovimentar;
        if(podeMovimentar)
        {
            jogoPausado = true;
        }
        break;
    }

}


void specialKey(int key, int x, int y)
{
    bool isOrthoAux = isOrtho;
    if(GLUT_KEY_F12 == key && !isFullScreen)
    {
        glutFullScreen();
        glutReshapeFunc(reshape);
        isFullScreen = true;
    }
    else if(GLUT_KEY_F12 == key && isFullScreen)
    {
        glutReshapeWindow(1000, 600);
        glutPositionWindow(0,0);
        glutReshapeFunc(reshape);
        isFullScreen = false;
    }
}

// Mouse callback

void mouse(int button, int state, int x, int y)
{
    if(!jogoPausado)
    {
        if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            last_x = x;
            last_y = y;
        }
        if ( button == GLUT_LEFT_BUTTON && !shooted)
        {
            ballPositionY = playerPositonY + 1;
            ballPositionX = playerPositionX;
            calculaVelocidadeBola(arrowAngle);
            shooted = true;
        }
        if(button == 3 && arrowAngle > -30) // Scroll up
        {
            arrowAngle -= 5;
            if(arrowAngle == 0)
            {
                arrowAngle = -5;
            }
        }
        if(button == 4 && arrowAngle < 30) // Scroll Down
        {
            arrowAngle += 5;
            if(arrowAngle == 0)
            {
                arrowAngle = 5;
            }
        }
    }
}

void mousePassive(int x, int y)
{
    if(!jogoPausado)
    {
        if(x > 500 && playerPositionX < 4.18)
        {
            playerPositionX += 0.15;
        }
        else if(x < 500 && playerPositionX > -4.18)
        {
            playerPositionX -= 0.15;
        }
    }
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
