#include <glad/glad.h>
#include <Renderer/Renderer2D.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "InputEvent/MouseEventHandler.h"
#include "Game/Game.h"

static int minorNumber = 3, majorNumber = 3;
static int windowWidth = 600, windowHeight = 600;
static const char *windowCaption = "gaming";
static GLFWwindow *window;

MouseEventHandler *mouseHandler = nullptr;

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    double pX, pY;
    glfwGetCursorPos(window, &pX, &pY);
    if (action == GLFW_PRESS)
        mouseHandler->onMousePressed((button == GLFW_MOUSE_BUTTON_LEFT) ? MOUSE_LEFT : MOUSE_RIGHT, (int)pX, (int)pY);
    else
        mouseHandler->onMouseReleased((button == GLFW_MOUSE_BUTTON_LEFT) ? MOUSE_LEFT : MOUSE_RIGHT, (int)pX, (int)pY);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorNumber);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorNumber);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, windowCaption, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /// -----------
    Renderer2D renderer;
    Game game = Game(renderer);
    mouseHandler = &game.GetMouseHandler();

    /// ------------------

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.Update();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}