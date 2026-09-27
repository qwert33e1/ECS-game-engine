#pragma once

#include "common.h"
#include "ECS/EntityManager.h"
#include "Game/Components/Projectile.h"
#include "Game/Components/Hostile.h"
#include "Game/Components/HealthPoint.h"
#include "Game/Components/DeadTag.h"

class DamageSystem
{
public:
    void Update(EntityManager &entityManager)
    {
        for (auto const &event : entityManager.GetCollisionEvents())
        {
            auto entity1 = event.entity1;
            auto entity2 = event.entity2;

            if (entityManager.HasComponent<Hostile>(entity1) && entityManager.HasComponent<Projectile>(entity2))
            {
                ResolveDamage(entityManager, entity1, entity2);
            }

            if (entityManager.HasComponent<Hostile>(entity2) && entityManager.HasComponent<Projectile>(entity1))
            {
                ResolveDamage(entityManager, entity2, entity1);
            }
        }
    }

    void ResolveDamage(EntityManager &entityManager, uint32_t hostileId, uint32_t projectileId)
    {
        Chunk *hostileChunk;
        uint32_t hostileIndex;

        Chunk *projectileChunk;
        uint32_t projectileIndex;

        if (!entityManager.GetEntityIndexInChunk(hostileId, hostileChunk, hostileIndex))
        {
            return;
        }

        if (!entityManager.GetEntityIndexInChunk(projectileId, projectileChunk, projectileIndex))
        {
            return;
        }

        uint32_t hpId = ComponentRegistry::GetId<HealthPoint>();
        uint32_t projId = ComponentRegistry::GetId<Projectile>();

        uint32_t hpSize = ComponentRegistry::globalSizeTable[hpId];
        uint32_t projSize = ComponentRegistry::globalSizeTable[projId];

        uint32_t hpOffset = hostileChunk->archetype->GetComponentOffset(hpId);
        uint32_t projOffset = projectileChunk->archetype->GetComponentOffset(projId);

        HealthPoint *hp = reinterpret_cast<HealthPoint *>(hostileChunk->data + hpOffset + hostileIndex * hpSize);
        Projectile *proj = reinterpret_cast<Projectile *>(projectileChunk->data + projOffset + hostileIndex * projSize);

        hp->current -= proj->damage;

        // printf("DEADTAG\n");
        entityManager.AddComponent<DeadTag>(projectileId, DeadTag{});

        if (hp->current <= 0)
        {
            entityManager.AddComponent<DeadTag>(hostileId, DeadTag{});
        }
    }
};