#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Sprite.h"
#include "Game/Components/Camera.h"
#include "Renderer/Renderer2D.h"
#include "Utility/Coordinates.h"
#include <glm/glm.hpp>

/// TODO: camera entity, pos conversion

class RenderSystem
{
    EntityManager &entityManager;
    Renderer2D &renderer;

public:
    RenderSystem(EntityManager &em, Renderer2D &renderer) : entityManager(em), renderer(renderer) {}

    void Draw()
    {
        // getting the camera entity
        uint32_t cameraId = ComponentRegistry::GetId<Camera>();
        uint32_t cameraSize = ComponentRegistry::globalSizeTable[cameraId];
        std::bitset<MAX_COMPONENTS> cameraMask;
        cameraMask.set(cameraId, true);
        auto cameraChunk = (entityManager.GetEntities(cameraMask)).front();
        if (cameraChunk->entityCounter == 0)
        {
            return;
        }
        uint32_t cameraOffset = cameraChunk->archetype->GetComponentOffset(cameraId);
        Camera *camera = reinterpret_cast<Camera *>(cameraChunk->data + cameraOffset);

        uint32_t posId = ComponentRegistry::GetId<Position>();
        uint32_t spriteId = ComponentRegistry::GetId<Sprite>();

        uint32_t posSize = ComponentRegistry::globalSizeTable[posId];
        uint32_t spriteSize = ComponentRegistry::globalSizeTable[spriteId];

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(posId, true);
        bitmask.set(spriteId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t posOffset = chunk->archetype->GetComponentOffset(posId);
            uint32_t spriteOffset = chunk->archetype->GetComponentOffset(spriteId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);
                Sprite *sprite = reinterpret_cast<Sprite *>(chunk->data + spriteOffset + i * spriteSize);

                glm::vec2 sPos = Coordinates::WorldToScreen(pos->x, pos->y, *camera);
                // printf("RENDERED COORDS: (%f, %f)\n", sPos.x, sPos.y);

                renderer.AddQuad(sPos, 50.0f, sprite->color);
            }
        }
        renderer.updateGPU();
        renderer.Draw();
        renderer.clear();
    }
};