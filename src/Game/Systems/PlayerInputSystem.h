#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Velocity.h"
#include "Game/Components/PlayerControlled.h"
#include "Game/Components/TargetPosition.h"
#include "Game/Components/Speed.h"
#include "Input/PlayerInputManager.h"
#include "Utility/Coordinates.h"
#include <glm/glm.hpp>

class PlayerInputSystem
{
public:
    void Update(EntityManager &entityManager, PlayerInputManager &inputManager)
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
        uint32_t cameraOffset = cameraChunk->archetype->getComponentOffset(cameraId);
        Camera *camera = reinterpret_cast<Camera *>(cameraChunk->data + cameraOffset);

        uint32_t posId = ComponentRegistry::GetId<Position>();
        uint32_t pcId = ComponentRegistry::GetId<PlayerControlled>();
        uint32_t targetId = ComponentRegistry::GetId<TargetPosition>();

        uint32_t posSize = ComponentRegistry::globalSizeTable[posId];
        uint32_t targetSize = ComponentRegistry::globalSizeTable[targetId];

        std::bitset<MAX_COMPONENTS> bitmask;

        bitmask.set(posId, true);
        bitmask.set(pcId, true);
        bitmask.set(targetId, true);

        std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

        for (auto chunk : entityChunks)
        {
            uint32_t posOffset = chunk->archetype->getComponentOffset(posId);
            uint32_t targetOffset = chunk->archetype->getComponentOffset(targetId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);
                TargetPosition *target = reinterpret_cast<TargetPosition *>(chunk->data + targetOffset + i * targetSize);

                glm::vec2 wPos = Coordinates::ScreenToWorld(inputManager.mouseX, inputManager.mouseY, *camera);

                if (inputManager.IsMouseButtonJustPressed(1))
                {
                    target->x = wPos.x;
                    target->y = wPos.y;

                    printf("SET TARGET :: sTarget : (%f, %f) - wTarget : (%f, %f) - pos : (%f, %f)\n ", inputManager.mouseX, inputManager.mouseY, wPos.x, wPos.y, pos->x, pos->y);
                }

                if (inputManager.IsKeyJustPressed('Q'))
                {
                    float dist = glm::length(glm::vec2(wPos.x - pos->x, wPos.y - pos->y));

                    entityManager.CreateEntity(Position{pos->x, pos->y}, Velocity{(wPos.x - pos->x) / dist * 10.0f, (wPos.y - pos->y) / dist * 10.0f}, Sprite{glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)});
                }
            }
        }
    }
};