/*
  Name:        triangle.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update: 07/09/2019
  Description: Simple opengl program
*/

#include <stdio.h>
#include <GL/glut.h>

void display(void);
void init (void);
void keyboard(unsigned char key, int x, int y);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Triangle Test");
    init ();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    printf("Pressione ESC para fechar.\n");

    glutMainLoop();

    return 0;
}

void desenhaEixos()
{
    glColor3f (0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex2f ( 0.0, -1.0);
        glVertex2f ( 0.0,  1.0);
        glVertex2f (-1.0,  0.0);
        glVertex2f ( 1.0,  0.0);
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

    // inicializar sistema de viz.
    glMatrixMode(GL_PROJECTION);              // Seleciona Matriz de Projeção
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity();           // Inicializa com matriz identidade
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
    }
    glutPostRedisplay();
}
