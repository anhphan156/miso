#include "window.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

void        error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
float       w = 800.0f;
float       h = 600.0f;

float GetAspectRatio() {
    return h / w;
}

GLFWwindow *GetWindow() {
    static GLFWwindow *window = NULL;

    if (window == NULL) {

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwSetErrorCallback(error_callback);

        window = glfwCreateWindow(w, h, "GLFW", NULL, NULL);
        if (window == NULL) {
            printf("Failed to create GLFW window");
            glfwTerminate();
            return NULL;
        }

        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);
    }

    return window;
}

void error_callback(int error, const char *description) {
    printf("Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
