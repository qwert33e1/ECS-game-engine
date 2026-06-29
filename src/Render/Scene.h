#pragma once
#include <algorithm>
#include "GameObject.h"
#include "Material.h"
#include <vector>

class Camera;
class TankAvatar;
class PhongShader;
class Tank;
class Square;

class Scene
{
    Camera *camera;
    Light light;
    PhongShader *shader;
    TankAvatar *avatar;
    bool tps = true;

public:
    std::vector<GameObject *> objects;
    std::vector<GameObject *> removableObjects;

    Scene();

    void Animate(float tstart, float tend);

    void Render();

    void ProcessInput(int key, bool down);
};