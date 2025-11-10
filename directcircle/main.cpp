#include <GL/glut.h>
#include<cmath>

int r = 50;

void dcd() {
    glColor3f(1,1,0);
    glPointSize(4);
    glBegin(GL_POINTS);

    for (int x = 0; x <= r; x++) {
        int y = (int)(sqrt(r * r - x * x));
        glVertex2i(x, y);
        glVertex2i(x, -y);
        glVertex2i(-x, -y);
        glVertex2i(-x, y);
    }
    glEnd();
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex2i(100, 0);
        glVertex2i(-100, 0);
        glVertex2i(0, 100);
        glVertex2i(0, -100);
    glEnd();
    dcd();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(4.0);
    gluOrtho2D(-100, 100, -100, 100);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("direct circle drawing");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
