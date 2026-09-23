#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Camera.h"
#include "Game/Components/Position.h"
#include "Game/Components/PlayerControlled.h"

class CameraFollowSystem
{
public:
    // assums there is only one PlayerControlled character and only one Camera
    void Update(EntityManager &entityManager)
    {
        uint32_t cameraId = ComponentRegistry::GetId<Camera>();
        uint32_t cameraSize = ComponentRegistry::globalSizeTable[cameraId];

        uint32_t posId = ComponentRegistry::GetId<Position>();
        uint32_t pcId = ComponentRegistry::GetId<PlayerControlled>();

        uint32_t posSize = ComponentRegistry::globalSizeTable[posId];

        std::bitset<MAX_COMPONENTS> cameraMask;
        std::bitset<MAX_COMPONENTS> playerMask;

        cameraMask.set(cameraId, true);
        playerMask.set(posId, true);
        playerMask.set(pcId, true);

        auto cameraChunk = (entityManager.GetEntities(cameraMask)).front();
        auto playerChunk = (entityManager.GetEntities(playerMask)).front();

        if (playerChunk->entityCounter == 0 || cameraChunk->entityCounter == 0)
        {
            return;
        }

        uint32_t cameraOffset = cameraChunk->archetype->getComponentOffset(cameraId);
        uint32_t posOffset = playerChunk->archetype->getComponentOffset(posId);

        Camera *camera = reinterpret_cast<Camera *>(cameraChunk->data + cameraOffset);
        Position *pos = reinterpret_cast<Position *>(playerChunk->data + posOffset);

        camera->x = pos->x;
        camera->y = pos->y;

        // printf("CAMERA: (%f, %f)\n", camera->x, camera->y);
    }
};