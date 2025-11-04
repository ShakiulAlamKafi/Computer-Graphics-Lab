// brick_breaker.cpp
// Simple Brick Breaker game using GLUT
// Compile: g++ brick_breaker.cpp -o brick_breaker -lGL -lGLU -lglut

#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

// Window
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Paddle
float paddleW = 120.0f;
float paddleH = 12.0f;
float paddleX = WINDOW_WIDTH / 2.0f - paddleW / 2.0f;
const float PADDLE_Y = 40.0f;
const float PADDLE_SPEED = 10.0f;

// Ball
float ballX = WINDOW_WIDTH / 2.0f;
float ballY = PADDLE_Y + paddleH + 10.0f;
float ballRadius = 8.0f;
float ballSpeed = 6.0f;
float ballVX = 0.0f, ballVY = 0.0f;
bool ballLaunched = false;

// Bricks
struct Brick { float x, y, w, h; bool alive; int hits; };
std::vector<Brick> bricks;
int BRICK_ROWS = 5;
int BRICK_COLS = 10;
float brickMargin = 5.0f;
float brickAreaTop = 120.0f;

// Game state
int score = 0;
int lives = 3;
bool gameOver = false;
bool gameWon = false;

// Utility: draw text
void drawText(float x, float y, const std::string &s) {
    glRasterPos2f(x, y);
    for (char c : s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void resetLevel() {
    bricks.clear();
    float areaW = WINDOW_WIDTH - 60.0f;
    float brickW = (areaW - (BRICK_COLS - 1) * brickMargin) / BRICK_COLS;
    float brickH = 20.0f;

    float startX = 30.0f;
    float startY = WINDOW_HEIGHT - brickAreaTop;

    for (int r = 0; r < BRICK_ROWS; ++r) {
        for (int c = 0; c < BRICK_COLS; ++c) {
            Brick b;
            b.w = brickW;
            b.h = brickH;
            b.x = startX + c * (brickW + brickMargin);
            b.y = startY - r * (brickH + brickMargin);
            b.alive = true;
            b.hits = 1; // increase for stronger bricks
            bricks.push_back(b);
        }
    }

    // Reset paddle and ball
    paddleX = WINDOW_WIDTH / 2.0f - paddleW / 2.0f;
    ballX = WINDOW_WIDTH / 2.0f;
    ballY = PADDLE_Y + paddleH + 10.0f;
    ballLaunched = false;
    ballVX = 0.0f;
    ballVY = 0.0f;
    score = 0;
    lives = 3;
    gameOver = false;
    gameWon = false;
}

void resetBallAfterLife() {
    ballX = paddleX + paddleW / 2.0f;
    ballY = PADDLE_Y + paddleH + 10.0f;
    ballLaunched = false;
    ballVX = 0.0f;
    ballVY = 0.0f;
}

void initGame() {
    resetLevel();
}

// Rendering functions
void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
      glVertex2f(x, y);
      glVertex2f(x + w, y);
      glVertex2f(x + w, y + h);
      glVertex2f(x, y + h);
    glEnd();
}

void drawCircle(float cx, float cy, float r, int segments = 32) {
    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(cx, cy);
      for (int i = 0; i <= segments; ++i) {
          float theta = (2.0f * 3.1415926f * float(i)) / float(segments);
          float x = r * cosf(theta);
          float y = r * sinf(theta);
          glVertex2f(cx + x, cy + y);
      }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw paddle
    glColor3f(0.2f, 0.6f, 1.0f); // blue
    drawRect(paddleX, PADDLE_Y, paddleW, paddleH);

    // Draw ball
    glColor3f(1.0f, 0.8f, 0.2f); // yellow
    drawCircle(ballX, ballY, ballRadius);

    // Draw bricks
    for (auto &b : bricks) {
        if (!b.alive) continue;
        // color varies by row (derivable from y)
        float rel = (b.y / WINDOW_HEIGHT);
        glColor3f(0.8f * rel + 0.2f, 0.2f, 0.6f * (1.0f - rel) + 0.2f);
        drawRect(b.x, b.y, b.w, b.h);
        // draw brick border
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
          glVertex2f(b.x, b.y);
          glVertex2f(b.x + b.w, b.y);
          glVertex2f(b.x + b.w, b.y + b.h);
          glVertex2f(b.x, b.y + b.h);
        glEnd();
    }

    // HUD: score and lives
    glColor3f(1.0f, 1.0f, 1.0f);
    std::ostringstream oss;
    oss << "Score: " << score;
    drawText(10, WINDOW_HEIGHT - 25, oss.str());
    oss.str(""); oss.clear();
    oss << "Lives: " << lives;
    drawText(WINDOW_WIDTH - 110, WINDOW_HEIGHT - 25, oss.str());

    // messages
    if (!ballLaunched && !gameOver && !gameWon) {
        drawText(WINDOW_WIDTH/2 - 90, 80, "Press SPACE to launch the ball");
    }
    if (gameOver) {
        drawText(WINDOW_WIDTH/2 - 60, WINDOW_HEIGHT/2, "GAME OVER");
        drawText(WINDOW_WIDTH/2 - 110, WINDOW_HEIGHT/2 - 30, "Press R to restart");
    }
    if (gameWon) {
        drawText(WINDOW_WIDTH/2 - 40, WINDOW_HEIGHT/2, "YOU WIN!");
        drawText(WINDOW_WIDTH/2 - 110, WINDOW_HEIGHT/2 - 30, "Press R to play again");
    }

    glutSwapBuffers();
}

// Collision helpers
bool circleRectCollision(float cx, float cy, float r, float rx, float ry, float rw, float rh) {
    // Find closest point on rectangle to circle center
    float closestX = std::fmax(rx, std::fmin(cx, rx + rw));
    float closestY = std::fmax(ry, std::fmin(cy, ry + rh));
    float dx = cx - closestX;
    float dy = cy - closestY;
    return (dx*dx + dy*dy) <= r*r;
}

void updateBall() {
    if (!ballLaunched || gameOver || gameWon) return;

    ballX += ballVX;
    ballY += ballVY;

    // Wall collisions (left, right)
    if (ballX - ballRadius <= 0) {
        ballX = ballRadius;
        ballVX = -ballVX;
    } else if (ballX + ballRadius >= WINDOW_WIDTH) {
        ballX = WINDOW_WIDTH - ballRadius;
        ballVX = -ballVX;
    }
    // Top wall
    if (ballY + ballRadius >= WINDOW_HEIGHT) {
        ballY = WINDOW_HEIGHT - ballRadius;
        ballVY = -ballVY;
    }

    // Bottom: lose life
    if (ballY - ballRadius <= 0) {
        lives--;
        if (lives <= 0) {
            gameOver = true;
            ballLaunched = false;
        } else {
            resetBallAfterLife();
        }
        return;
    }

    // Paddle collision
    if (circleRectCollision(ballX, ballY, ballRadius, paddleX, PADDLE_Y, paddleW, paddleH)) {
        // reflect vertically
        ballY = PADDLE_Y + paddleH + ballRadius + 0.1f;
        ballVY = fabs(ballVY); // ensure positive upward (we use standard upward positive VY)
        // adjust X velocity based on where it hit the paddle
        float hitPos = (ballX - (paddleX + paddleW/2.0f)) / (paddleW/2.0f); // -1..1
        float maxAngle = 75.0f * (3.1415926f / 180.0f); // in radians
        float angle = hitPos * maxAngle;
        float speed = sqrt(ballVX*ballVX + ballVY*ballVY);
        ballVX = speed * sin(angle);
        ballVY = speed * cos(angle);
        if (ballVY < 2.0f) ballVY = 2.0f; // avoid too shallow
        // ensure ball goes upward
        ballVY = fabs(ballVY);
    }

    // Brick collisions
    for (auto &b : bricks) {
        if (!b.alive) continue;
        if (circleRectCollision(ballX, ballY, ballRadius, b.x, b.y, b.w, b.h)) {
            // basic resolution: determine side of impact by previous position
            // approximate by checking overlap depths
            float overlapLeft = (ballX + ballRadius) - b.x;
            float overlapRight = (b.x + b.w) - (ballX - ballRadius);
            float overlapBottom = (ballY + ballRadius) - b.y;
            float overlapTop = (b.y + b.h) - (ballY - ballRadius);

            bool ballFromLeft = overlapLeft < overlapRight;
            bool ballFromBottom = overlapBottom < overlapTop;

            float minOverlapX = ballFromLeft ? overlapLeft : overlapRight;
            float minOverlapY = ballFromBottom ? overlapBottom : overlapTop;

            if (minOverlapX < minOverlapY) {
                // reflect X
                ballVX = -ballVX;
            } else {
                // reflect Y
                ballVY = -ballVY;
            }

            b.hits--;
            if (b.hits <= 0) {
                b.alive = false;
                score += 10;
            } else {
                score += 5;
            }

            // check win (all bricks destroyed)
            bool anyAlive = false;
            for (auto &bb : bricks) { if (bb.alive) { anyAlive = true; break; } }
            if (!anyAlive) {
                gameWon = true;
                ballLaunched = false;
            }

            break; // only collide one brick per frame to avoid weird tunneling
        }
    }
}

void timer(int val) {
    // Called at approximately 60 FPS
    updateBall();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Input handlers
void specialKeys(int key, int x, int y) {
    if (gameOver || gameWon) return;
    if (key == GLUT_KEY_LEFT) {
        paddleX -= PADDLE_SPEED;
        if (paddleX < 0) paddleX = 0;
        if (!ballLaunched) { // move ball with paddle pre-launch
            ballX = paddleX + paddleW / 2.0f;
        }
    } else if (key == GLUT_KEY_RIGHT) {
        paddleX += PADDLE_SPEED;
        if (paddleX + paddleW > WINDOW_WIDTH) paddleX = WINDOW_WIDTH - paddleW;
        if (!ballLaunched) { // move ball with paddle pre-launch
            ballX = paddleX + paddleW / 2.0f;
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC
    if (key == 'r' || key == 'R') {
        resetLevel();
        return;
    }
    if (key == ' ' && !gameOver && !gameWon) {
        if (!ballLaunched) {
            // launch ball with initial speed upward-left or right randomly
            ballLaunched = true;
            float angle = (45.0f + (rand() % 30 - 15)) * (3.1415926f/180.0f); // around 45 degrees
            ballVX = ballSpeed * cos(angle) * ((rand()%2) ? 1.0f : -1.0f);
            ballVY = ballSpeed * sin(angle);
            // ensure ballVY is upward (positive)
            if (ballVY < 0) ballVY = -ballVY;
        }
    }
}

void reshape(int w, int h) {
    // Keep viewport fixed to initial window size to avoid complexity
    glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Brick Breaker - GLUT C++");

    // Set up orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    initGame();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
