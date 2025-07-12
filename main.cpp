#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include "maze.h"

const int WIDTH = 800, HEIGHT = 600;
Player player;

float radians(float degrees) {
    return degrees * 3.14159265f / 180.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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

        updatePlayer(player, window);
        setupLighting(player);

        // Set up projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (GLdouble)WIDTH/(GLdouble)HEIGHT, 0.1, 100.0);

        // Set up view matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Calculate look-at point
        float lookX = player.x + cosf(radians(player.yaw)) * cosf(radians(player.pitch));
        float lookY = player.y + sinf(radians(player.pitch));
        float lookZ = player.z + sinf(radians(player.yaw)) * cosf(radians(player.pitch));
        
        // Up vector is always (0,1,0) for standard FPS controls
        gluLookAt(
            player.x, player.y, player.z,  // Eye position
            lookX, lookY, lookZ,          // Look-at point
            0.0, 1.0, 0.0                 // Up vector (this was missing)
        );

        drawMaze(player);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}