#include <GL/glut.h>


void translate()
{
    float x1 = 4.0f, y1 = 6.0f;
    float x2 = 2.0f, y2 = 2.0f;
    float x3 = 6.0f, y3 = 2.0f;

    float tx = 4.0f;
    float ty = 5.0f;

    float x1_t = x1 + tx, y1_t = y1 + ty;
    float x2_t = x2 + tx, y2_t = y2 + ty;
    float x3_t = x3 + tx, y3_t = y3 + ty;


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
    translate();
    glutSwapBuffers();

}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("2d translate");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-15, 15, -15, 15);
    glMatrixMode(GL_MODELVIEW);

    glutDisplayFunc(display);

    glutMainLoop();

    return EXIT_SUCCESS;
}
