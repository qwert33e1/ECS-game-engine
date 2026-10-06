#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Sprite.h"
#include "Game/Components/Camera.h"
#include "DataStructures/SpriteManager.h"
#include "Renderer/Renderer2D.h"
#include "Utility/Coordinates.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>

class RenderSystem
{
    Renderer2D &renderer;
    SpriteManager spriteManager;
    Texture background;

public:
    RenderSystem(Renderer2D &renderer) : renderer(renderer), background("Textures/grass.png") {}

    void LoadTexture();

    void Update(EntityManager &entityManager);
};