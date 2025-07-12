#include "maze.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

int maze[MAZE_SIZE][MAZE_SIZE] = {
    {1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,1,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,1,0,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1},
    {1,1,1,0,1,0,1,1,0,1,1,1,0,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,1,1,1,1,0,1,1,0,1,1,1,0,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void initMaze() {}

void drawWall(float x1, float y1, float z1, float x2, float y2, float z2) {
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.5f, 0.3f);
    glVertex3f(x1, y1, z1); glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1); glVertex3f(x1, y2, z1);
    glVertex3f(x1, y1, z2); glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2); glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z1); glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2); glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z1); glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2); glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1); glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2); glVertex3f(x1, y2, z2);
    glEnd();
}

void drawMaze(Player& player) {
    const float cellSize = 4.0f;
    
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (maze[i][j] == 1) {
                float x = j * cellSize;
                float z = -i * cellSize;
                drawWall(x, 0.0f, z, x + cellSize, WALL_HEIGHT, z);
                drawWall(x, 0.0f, z - cellSize, x + cellSize, WALL_HEIGHT, z - cellSize);
                drawWall(x, 0.0f, z, x, WALL_HEIGHT, z - cellSize);
                drawWall(x + cellSize, 0.0f, z, x + cellSize, WALL_HEIGHT, z - cellSize);
            }
        }
    }
    
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(MAZE_SIZE * cellSize, 0.0f, 0.0f);
    glVertex3f(MAZE_SIZE * cellSize, 0.0f, -MAZE_SIZE * cellSize);
    glVertex3f(0.0f, 0.0f, -MAZE_SIZE * cellSize);
    glEnd();
}

void updatePlayer(Player& player, GLFWwindow* window) {
    float moveSpeed = player.speed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player.x += cosf(radians(player.yaw)) * moveSpeed;
        player.z += sinf(radians(player.yaw)) * moveSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player.x -= cosf(radians(player.yaw)) * moveSpeed;
        player.z -= sinf(radians(player.yaw)) * moveSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player.x -= cosf(radians(player.yaw + 90)) * moveSpeed;
        player.z -= sinf(radians(player.yaw + 90)) * moveSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player.x += cosf(radians(player.yaw + 90)) * moveSpeed;
        player.z += sinf(radians(player.yaw + 90)) * moveSpeed;
    }
    
    int cellX = static_cast<int>(player.x / 4.0f);
    int cellZ = static_cast<int>(-player.z / 4.0f);
    
    if (cellX >= 0 && cellX < MAZE_SIZE && cellZ >= 0 && cellZ < MAZE_SIZE) {
        if (maze[cellZ][cellX] == 1) {
            player.x -= cosf(radians(player.yaw)) * moveSpeed * 2.0f;
            player.z -= sinf(radians(player.yaw)) * moveSpeed * 2.0f;
        }
    }
}

void setupLighting(Player& player) {
    GLfloat lightPos[] = {player.x, player.y + 1.0f, player.z, 1.0f};
    GLfloat lightDir[] = {
        static_cast<GLfloat>(cosf(radians(player.yaw)) * cosf(radians(player.pitch))),
        static_cast<GLfloat>(sinf(radians(player.pitch))),
        static_cast<GLfloat>(sinf(radians(player.yaw)) * cosf(radians(player.pitch)))
    };
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);
    
    GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuse[] = {0.8f, 0.6f, 0.4f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
}