#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(){
    glfwInit();
    /* glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); */
    /* glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); */
    /* glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); // Set the client API */
    /* glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API); */
    glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND); 
    /* glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); */

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    GLenum err = glewInit();
    /* if(err != GLEW_OK){ */
    /*     std::cout << err  << std::endl; */
    /* } */
    /* std::cout << glGetString(GL_VERSION) << std::endl; */

    float positions[6] = {
        -.5f, -.5f,
        0.0f, 0.2f,
        .5f, -.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);

    int shader_program = create_shader_program();
    glUseProgram(shader_program);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &buffer);
    glfwTerminate();

    return 0;
}

void error_callback(int error, const char* description)
{
    printf("Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}