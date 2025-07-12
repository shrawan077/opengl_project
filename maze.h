#ifndef MAZE_H
#define MAZE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MAZE_SIZE 20
#define WALL_HEIGHT 5.0f
#define TORCH_RANGE 15.0f

extern int maze[MAZE_SIZE][MAZE_SIZE];

struct Player {
    float x = 2.0f * 4.0f;
    float y = 1.0f;
    float z = 0.0f * 4.0f;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 0.1f;
};

void initMaze();
void drawWall(float x1, float y1, float z1, float x2, float y2, float z2);
void drawMaze(Player& player);
void updatePlayer(Player& player, GLFWwindow* window);
void setupLighting(Player& player);
float radians(float degrees);

#endif