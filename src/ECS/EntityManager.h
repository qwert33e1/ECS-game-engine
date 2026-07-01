#pragma once

#include "common.h"
#include "Archetype.h"
#include "ComponentRegistry.h"

class EntityManager
{
private:
    std::queue<uint32_t> idPool;
    std::vector<std::unique_ptr<Chunk>> chunkPool;
    std::unordered_map<std::bitset<MAX_COMPONENTS>, std::unique_ptr<Archetype>> archMap;
    std::unordered_map<uint32_t, std::bitset<MAX_COMPONENTS>> entitySignatureMap;

    uint32_t getNextId()
    {
        uint32_t ret = idPool.front();
        idPool.pop();
        return ret;
    }

public:
    EntityManager()
    {
        for (uint32_t i = 0; i < MAX_ENTITIES; i++)
        {
            idPool.push(i);
        }
    }

    template <typename... Ts>
    uint32_t CreateEntity(Ts &&...components)
    {
        std::bitset<MAX_COMPONENTS> signature;

        (signature.set(ComponentRegistry::GetId<Ts>(), true), ...);

        if (!archMap.contains(signature))
        {
            archMap[signature] = std::make_unique<Archetype>(signature, chunkPool);
        }

        uint32_t newEntityId = getNextId();

        archMap[signature]->AddEntity(newEntityId, std::forward<Ts>(components)...);
        entitySignatureMap[newEntityId] = signature;

        return newEntityId;
    }

    void DestroyEntity(uint32_t entityId)
    {
        archMap[entitySignatureMap[entityId]]->RemoveEntity(entityId);

        entitySignatureMap.erase(entityId);
    }
};