#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Sprite.h"
#include "Game/Components/Camera.h"
#include "Renderer/Renderer2D.h"
#include "Utility/Coordinates.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>

class RenderSystem
{
    Renderer2D &renderer;
    Texture background;
    Texture spriteSheet;

public:
    RenderSystem(Renderer2D &renderer) : renderer(renderer), background("Textures/feelsbadman2mask.png"), spriteSheet("Textures/feelsbadman2mask.png") {}

    void Update(EntityManager &entityManager);
};