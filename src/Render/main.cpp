#include "Scene.h"
#include "common.h"

class GameApp : public glApp
{
    Scene *scene;

public:
    GameApp() : glApp("Gaming") {}

    void onInitialization()
    {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        scene = new Scene();
    }
    void onDisplay()
    {
        glClearColor(0.4f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->Render();
    }

    void onKeyboard(int key) override
    {
        scene->ProcessInput(key, true);
    }

    void onKeyboardUp(int key) override
    {
        scene->ProcessInput(key, false);
    }

    void onTimeElapsed(float tstart, float tend)
    {
        scene->Animate(tstart, tend);
        refreshScreen();
    }
} app;