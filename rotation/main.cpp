#include <GL/glut.h>
#include <math.h>

void rotation()
{
    float x1 = 4.0f, y1 = 6.0f;
    float x2 = 2.0f, y2 = 2.0f;
    float x3 = 6.0f, y3 = 2.0f;

    float theta=45;
    float rad=theta*3.1416/180;

    float x1_t = x1 * cos(rad)-y1*sin(rad), y1_t = y1 * cos(rad)+x1*sin(rad);
    float x2_t = x2 * cos(rad)-y2*sin(rad), y2_t = y2 * cos(rad)+x2*sin(rad);
    float x3_t = x3 * cos(rad)-y3*sin(rad), y3_t = y3 * cos(rad)+x3*sin(rad);


    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);

    glVertex2f(x2, y2);
    glVertex2f(x3, y3);

    glVertex2f(x3, y3);
    glVertex2f(x1, y1);
    glEnd();


    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex2f(x1_t, y1_t);
    glVertex2f(x2_t, y2_t);

    glVertex2f(x2_t, y2_t);
    glVertex2f(x3_t, y3_t);

    glVertex2f(x3_t, y3_t);
    glVertex2f(x1_t, y1_t);
    glEnd();
}



static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rotation();
    glutSwapBuffers();

}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("2d Rotation");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-15, 15, -15, 15);
    glMatrixMode(GL_MODELVIEW);

    glutDisplayFunc(display);

    glutMainLoop();

    return EXIT_SUCCESS;
}
