#include "Scene.h"
#include "Camera.h"
#include "Shader.h"
#include "Tank.h"
#include "Geometry.h"

Scene::Scene()
{
    camera = new Camera();
    shader = new PhongShader();

    Tank *tank = new Tank(shader);
    objects.push_back(tank);

    avatar = new TankAvatar(tank, this);

    Square *s = new Square();
    s->width = 1000;
    s->length = 1000;
    s->create(1, 1);
    MeshGameObject *ground = new MeshGameObject(s);
    ground->shader = shader;
    objects.push_back(ground);

    Material *mat = new Material();
    mat->ka = vec3(0.1f, 0.5f, 0.1f);
    mat->kd = vec3(0.2f, 0.8f, 0.2f);
    mat->ks = vec3(1.0f, 1.0f, 1.0f);
    mat->shine = 50.0f;

    camera->wEye = vec3(100.0f, 100.0f, 100.0f);
    camera->wLookat = vec3(0.0f, 0.0f, 0.0f);
    camera->wVup = vec3(0.0f, 1.0f, 0.0f);

    light.wLightPos = vec4(5.0f, 5.0f, 5.0f, 1.0f);
    light.La = vec3(0.4f, 0.4f, 0.4f);
    light.Le = vec3(1.0f, 1.0f, 1.0f);
}

void Scene::Animate(float tstart, float tend)
{
    for (GameObject *obj : objects)
    {
        if (obj->alive)
        {
            obj->Animate(tstart, tend);
        }
    }

    for (GameObject *&obj : objects)
    {
        if (obj && !obj->alive)
        {
            delete obj;
            obj = nullptr;
        }
    }

    objects.erase(
        std::remove_if(objects.begin(), objects.end(), [](GameObject *obj)
                       { return obj == nullptr; }),
        objects.end());
    avatar->updateCamera();
}

void Scene::Render()
{
    RenderState state;
    if (tps)
    {
        state.wEye = avatar->position;
        state.V = avatar->V();
        state.P = avatar->P();
    }
    else
    {
        state.wEye = camera->wEye;
        state.V = camera->V();
        state.P = camera->P();
    }
    state.light = light;

    state.M = mat4(1.0f);

    state.Minv = mat4(1.0f);

    for (GameObject *obj : objects)
    {
        obj->Draw(state);
    }
}

void Scene::ProcessInput(int key, bool down)
{
    if (key == 'v' && down)
    {
        tps = !tps;
    }
    avatar->ProcessInput(key, down);
}