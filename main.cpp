#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include "maze.h"

const int WIDTH = 800, HEIGHT = 600;
Player player;
bool mouseCaptured = true;
bool windowMaximized = false;

float radians(float degrees) {
    return degrees * 3.14159265f / 180.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!mouseCaptured) return;

    static float lastX = WIDTH / 2.0f;
    static float lastY = HEIGHT / 2.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    player.yaw += xoffset;
    player.pitch += yoffset;

    if (player.pitch > 89.0f) player.pitch = 89.0f;
    if (player.pitch < -89.0f) player.pitch = -89.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        mouseCaptured = !mouseCaptured;
        glfwSetInputMode(window, GLFW_CURSOR, 
                        mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
    
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        windowMaximized = !windowMaximized;
        if (windowMaximized) {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            glfwSetWindowMonitor(window, NULL, 100, 100, WIDTH, HEIGHT, 0);
        }
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D Maze Explorer", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    initMaze();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Movement controls
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
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            player.y += moveSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            player.y -= moveSpeed;
        }

        setupLighting(player);

        // Set up projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (GLdouble)WIDTH/(GLdouble)HEIGHT, 0.1, 100.0);

        // Set up view matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        float lookX = player.x + cosf(radians(player.yaw)) * cosf(radians(player.pitch));
        float lookY = player.y + sinf(radians(player.pitch));
        float lookZ = player.z + sinf(radians(player.yaw)) * cosf(radians(player.pitch));
        
        gluLookAt(
            player.x, player.y, player.z,
            lookX, lookY, lookZ,
            0.0, 1.0, 0.0
        );

        drawMaze(player);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}