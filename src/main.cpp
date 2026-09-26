#include <glad/glad.h>
#include <Renderer/Renderer2D.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Input/MouseEventHandler.h"
#include "Game/Game.h"
#include "Input/PlayerInputManager.h"

static int minorNumber = 3, majorNumber = 3;
static int windowWidth = WINDOW_WIDTH, windowHeight = WINDOW_HEIGHT;
static const char *windowCaption = "gaming";
static GLFWwindow *window;

PlayerInputManager inputManager;

MouseEventHandler *mouseHandler = nullptr;

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (action == GLFW_PRESS)
    {
        inputManager.PressMouseButton(button, x, y);
    }
    else if (action == GLFW_RELEASE)
    {
        inputManager.ReleaseMouseButton(button, x, y);
    }
}

void cursor_pos_callback(GLFWwindow *window, double x, double y)
{
    inputManager.SetMousePosition(x, y);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        inputManager.PressKey(key);
    printf("KEY WAS PRESSED: %d\n", key);
    if (action == GLFW_RELEASE)
        inputManager.ReleaseKey(key);
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
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /// -----------
    Renderer2D renderer;
    Game game = Game(renderer, inputManager);
    mouseHandler = &game.GetMouseHandler();

    /// ------------------

    while (!glfwWindowShouldClose(window))
    {
        inputManager.Update();
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