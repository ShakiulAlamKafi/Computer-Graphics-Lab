#include <GL/glut.h>
#include <iostream>

// Window size
const int WIDTH = 800;
const int HEIGHT = 600;

// Object properties
float objX = 0.0f;
float objY = 0.0f;
float objSize = 50.0f; // half width of the square
float moveStep = 10.0f;

// Draw the object (a simple square)
void drawObject() {
    glColor3f(0.2f, 0.8f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(objX - objSize, objY - objSize);
        glVertex2f(objX + objSize, objY - objSize);
        glVertex2f(objX + objSize, objY + objSize);
        glVertex2f(objX - objSize, objY + objSize);
    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawObject();
    glutSwapBuffers();
}

// Check if the object has reached boundary
bool isAtBoundary() {
    return (objX + objSize >= WIDTH / 2 ||
            objX - objSize <= -WIDTH / 2 ||
            objY + objSize >= HEIGHT / 2 ||
            objY - objSize <= -HEIGHT / 2);
}

// Keyboard input handling
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:    objY += moveStep; break;
        case GLUT_KEY_DOWN:  objY -= moveStep; break;
        case GLUT_KEY_LEFT:  objX -= moveStep; break;
        case GLUT_KEY_RIGHT: objX += moveStep; break;
    }

    // Check boundary condition
    if (isAtBoundary()) {
        std::cout << "Object reached boundary! Exiting...\n";
        exit(0);
    }

    glutPostRedisplay();
}

// Initialization
void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Background color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Move Object with Keyboard (GLUT)");

    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}
