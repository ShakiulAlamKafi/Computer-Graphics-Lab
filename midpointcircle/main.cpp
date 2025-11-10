#include <GL/glut.h>
#include<cmath>

int r = 50;

void mcd()
{
    int x=0;
    int y=r;
    int d=1-r;
    glColor3f(1,1,0);
    glPointSize(4);
    glBegin(GL_POINTS);

    while(x<=y)
    {
        glVertex2i(x, y);
        glVertex2i(y, x);
        glVertex2i(y, -x);
        glVertex2i(x, -y);
        glVertex2i(-x, -y);
        glVertex2i(-y, -x);
        glVertex2i(-y, x);
        glVertex2i(-x, y);

        if (d<=0)
        {
            x+=1;
            d+=2*x+3;

        }
        else
        {
            y-=1;
            x+=1;
            d+=2*x-2*y+5;
        }
    }

    glEnd();
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex2i(100, 0);
        glVertex2i(-100, 0);
        glVertex2i(0, 100);
        glVertex2i(0, -100);
    glEnd();
    mcd();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(3.0);
    gluOrtho2D(-100, 100, -100, 100);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("midpoint circle drawing");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
