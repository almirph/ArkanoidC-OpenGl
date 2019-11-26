/*
  Name:        triangle.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update: 07/09/2019
  Description: Colored triangle with reshape function enabled.
*/

#include <stdio.h>
#include <GL/glut.h>

void display(void);
void init (void);
void keyboard(unsigned char key, int x, int y);
void reshape(int, int);

bool fullScreen = false;
int width = 500, height = 500;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Triangle Test");
    init ();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    printf("Pressione ESC para fechar.\n");

    glutMainLoop();

    return 0;
}

void reshape (int w, int h)
{
    // Guarda informações de altura e largura do modo janela
    if(!fullScreen)
    {
        width = w;
        height = h;
    }

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    if (w <= h)
        glOrtho (-1, 1, -1*(GLfloat)h/(GLfloat)w, 1*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
    else
        glOrtho (-1*(GLfloat)w/(GLfloat)h, 1*(GLfloat)w/(GLfloat)h, -1, 1, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void desenhaEixos()
{
    glColor3f (0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex2f ( 0.0, -10.0);
        glVertex2f ( 0.0,  10.0);
        glVertex2f (-10.0,  0.0);
        glVertex2f ( 10.0,  0.0);
    glEnd();
}

void display(void)
{
    // Limpar todos os pixels
    glClear (GL_COLOR_BUFFER_BIT);

    // Função opcional, para desenhar os eixos do R2
    desenhaEixos();

    // Desenhar um polígono branco (retângulo)
    glBegin(GL_TRIANGLES);
        glColor3f (1.0, 0.0, 0.0);
        glVertex2f ( 0.0,  0.5);
        glColor3f (1.0, 1.0, 1.0);
        glVertex2f (-0.5, -0.5);
        glVertex2f ( 0.5, -0.5);
    glEnd();

    glutSwapBuffers ();
}

void init (void)
{
    // selecionar cor de fundo (preto)
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'f':
            fullScreen = !fullScreen;
            if (fullScreen)
            {
                glutFullScreen();
            }
            else
            {
                glutReshapeWindow(width, height);
            }
            break;
    }
    glutPostRedisplay();
}
