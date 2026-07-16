#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Position.h"
#include "Game/Components/Sprite.h"
#include "Renderer/Renderer2D.h"
#include <glm/glm.hpp>

class RenderSystem
{
    EntityManager &entityManager;
    Renderer2D &renderer;

public:
    RenderSystem(EntityManager em, Renderer2D renderer) : entityManager(em), renderer(renderer) {}

    void Draw()
    {
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
            uint32_t maxEntityInChunk = chunk->archetype->getMaxEntityInChunk();
            uint32_t posOffset = chunk->archetype->getComponentOffset(posId);
            uint32_t spriteOffset = chunk->archetype->getComponentOffset(spriteId);
            uint32_t entityCount = chunk->entityCounter;

            for (uint32_t i = 0; i < entityCount; i++)
            {
                Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);
                Sprite *sprite = reinterpret_cast<Sprite *>(chunk->data + spriteOffset + i * spriteSize);

                renderer.AddQuad(glm::vec2(pos->x, pos->y), 100.0f, sprite->color);
            }
        }
    }
};