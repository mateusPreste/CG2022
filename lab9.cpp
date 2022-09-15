#include <GL/glut.h>
#include <iostream>
#include <cmath>

const double PI = 3.141592653589793238463;

typedef struct coordenada
{
    double x;
    double y;
} coordenada;

typedef struct quadrado
{
    coordenada p1;
    coordenada p2;
    coordenada p3;
    coordenada p4;
} quadrado;

double x_pos = -1.0;
double y_pos = 9.0;
double escala = 0.99;
int passos = 6*15;
int pulseInterval = 6*10;
double currentScale = escala;
double aux;
int cont = 0;

quadrado *q;

quadrado *criaQuadrado ()
{
    quadrado *novo = (quadrado*)malloc(sizeof(quadrado));
    novo->p1.x = x_pos;
    novo->p1.y = y_pos;
    novo->p2.x = x_pos;
    novo->p2.y = y_pos-1.0;
    novo->p3.x = x_pos+1.0;
    novo->p3.y = y_pos-1.0;
    novo->p4.x = x_pos+1.0;
    novo->p4.y = y_pos;
    return novo;
}

coordenada *calculaCentro(quadrado *q1)
{
    coordenada *centro = (coordenada*) malloc(sizeof(coordenada));
    centro->x = (q1->p1.x + q1->p2.x + q1->p3.x + q1->p4.x)/4.0;
    centro->y = (q1->p1.y + q1->p2.y + q1->p3.y + q1->p4.y)/4.0;
    return centro;
}

void desenhaQuadrado (quadrado* q1)
{
    glBegin(GL_POLYGON);
    glVertex2f(q1->p1.x, q1->p1.y);
    glVertex2f(q1->p2.x, q1->p2.y);
    glVertex2f(q1->p3.x, q1->p3.y);
    glVertex2f(q1->p4.x, q1->p4.y);
    glEnd();
}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLoadIdentity();

    desenhaQuadrado(q);
 
    glutSwapBuffers();
}

void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
}

void inicializa (void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0.0, 200.0, 0.0, 200.0);

    q = criaQuadrado();
}

void translacao_origem(double d1, double d2)
{
    q->p1.x = q->p1.x + d1;
    q->p1.y = q->p1.y + d2;
    q->p2.x = q->p2.x + d1;
    q->p2.y = q->p2.y + d2;
    q->p3.x = q->p3.x + d1;
    q->p3.y = q->p3.y + d2;
    q->p4.x = q->p4.x + d1;
    q->p4.y = q->p4.y + d2;
}

void Multiply(double N[2][2], quadrado *q, double Result[2][4])
{
    double* M[2][4] = {{&(q->p1.x), &(q->p2.x), &(q->p3.x), &(q->p4.x)}, {&(q->p1.y), &(q->p2.y), &(q->p3.y), &(q->p4.y)}};

    for (int I = 0; I < 2; ++I)
    {
        for (int J = 0; J < 4; ++J)
        {
            double SumElements = 0.0f;
            for (int K = 0; K < 2; ++K)
            {
                SumElements += N[I][K] * *(M[K][J]);
            }
            Result[I][J] = SumElements;
        }
    }

    for (int a = 0; a < 2; a++){
        for(int b = 0; b < 4; b++){
            *(M[a][b]) = Result[a][b];
        }
    }
}

double degreeToRad(int degree){
    return (2*PI*degree)/360.0;
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(1000/165, timer, 0);
    
    coordenada *c = calculaCentro(q);
    
    //if(cont == 1){
    //    cont = 0;
    //    translacao_origem(-c->x, -c->y);
    //
    //
    //    double T [2][2] = {{1/std::pow(escala, 1), 0},{0, 1/std::pow(escala, 1)}};
    //    double result [2][4] = {};
    //    Multiply(T, q, result);
    //
    //    
    //    translacao_origem(c->x, c->y);
    //}
    
    translacao_origem(-c->x, -c->y);

    double T [2][2] = {{cos(degreeToRad(360/passos)), sin(degreeToRad(360/passos))},
                      {-sin(degreeToRad(360/passos)), cos(degreeToRad(360/passos))}};
    double result [2][4] = {};

    Multiply(T, q, result);
    
    //rotação em torno do proprio eixo
    //aux = q->p1.x;
    //q->p1.x = (q->p1.x*0.866+q->p1.y*0.5);
    //q->p1.y = (aux*(-0.5)+q->p1.y*0.866);
    //aux = q->p2.x;
    //q->p2.x = (q->p2.x*0.866+q->p2.y*0.5);
    //q->p2.y = (aux*(-0.5)+q->p2.y*0.866);
    //aux = q->p3.x;
    //q->p3.x = (q->p3.x*0.866+q->p3.y*0.5);
    //q->p3.y = (aux*(-0.5)+q->p3.y*0.866);
    //aux = q->p4.x;
    //q->p4.x = (q->p4.x*0.866+q->p4.y*0.5);
    //q->p4.y = (aux*(-0.5)+q->p4.y*0.866);

    

    if(cont % pulseInterval == 0){
        if(currentScale == escala){
            currentScale = 1/escala;
        } else{
            currentScale = escala;
        }
    }

    //std::cout << currentScale << std::endl;

    double T1 [2][2] = {{currentScale, 0},{0, currentScale}};
    double result1 [2][4] = {};

    Multiply(T1, q, result1);

    //q->p1.x = q->p1.x * escala;
    //q->p1.y = q->p1.y * escala;
    //q->p2.x = q->p2.x * escala;
    //q->p2.y = q->p2.y * escala;
    //q->p3.x = q->p3.x * escala;
    //q->p3.y = q->p3.y * escala;
    //q->p4.x = q->p4.x * escala;
    //q->p4.y = q->p4.y * escala;
    translacao_origem(c->x, c->y);
    
    double T2 [2][2] = {{cos(degreeToRad(360/(passos*4))), sin(degreeToRad(360/(passos*4)))},
                      {-sin(degreeToRad(360/(passos*4))), cos(degreeToRad(360/(passos*4)))}};
    double result2 [2][4] = {};

    Multiply(T2, q, result2);

    //rotacao em volta da origem
    //aux = q->p1.x;
    //q->p1.x = (q->p1.x*0.9659+q->p1.y*0.2588);
    //q->p1.y = (aux*(-0.2588)+q->p1.y*0.9659);
    //aux = q->p2.x;
    //q->p2.x = (q->p2.x*0.9659+q->p2.y*0.2588);
    //q->p2.y = (aux*(-0.2588)+q->p2.y*0.9659);
    //aux = q->p3.x;
    //q->p3.x = (q->p3.x*0.9659+q->p3.y*0.2588);
    //q->p3.y = (aux*(-0.2588)+q->p3.y*0.9659);
    //aux = q->p4.x;
    //q->p4.x = (q->p4.x*0.9659+q->p4.y*0.2588);
    //q->p4.y = (aux*(-0.2588)+q->p4.y*0.9659);
    
    cont = cont + 1;
}

int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Questao 19");
    inicializa();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}