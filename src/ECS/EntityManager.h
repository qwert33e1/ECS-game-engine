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
            archMap[signature] = std::unique_ptr<Archetype>(new Archetype(signature, chunkPool);
        }

        uint32_t newEntityId = getNextId();

        Archetype[signature].AddEntity(newEntityId, std::forward<Ts>(components)...);

        return newEntityId;
    }
};