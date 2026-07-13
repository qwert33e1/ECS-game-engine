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

        idPool.push(entityId);
    }

    template <typename T>
    void AddComponent(uint32_t entityId, T component)
    {
        std::bitset<MAX_COMPONENTS> oldSignature = entitySignatureMap[entityId];
        std::vector<uint8_t> oldComponents = archMap[oldSignature]->GetEntityComponents(entityId, oldSignature);

        if (oldComponents.size() == 0 && oldSignature.count() != 0)
        {
            return;
        }

        archMap[oldSignature]->RemoveEntity(entityId);

        std::bitset<MAX_COMPONENTS> newSignature = oldSignature;
        newSignature.set(ComponentRegistry::GetId<T>(), true);

        archMap[newSignature]->AddEntity<T>(entityId, oldSignature, oldComponents, component);

        entitySignatureMap[entityId] = newSignature;
    }

    template <typename T>
    void RemoveComponent(uint32_t entityId)
    {
        std::bitset<MAX_COMPONENTS> oldSignature = entitySignatureMap[entityId];
        std::bitset<MAX_COMPONENTS> newSignature = oldSignature;
        newSignature.set(ComponentRegistry::GetId<T>(), false);

        // the removable component wont be in the vector
        std::vector<uint8_t> oldComponents = archMap[oldSignature]->GetEntityComponents(entityId, newSignature);

        if (oldComponents.size() == 0 && newSignature.count() != 0)
        {
            return;
        }

        archMap[oldSignature]->RemoveEntity(entityId);

        archMap[newSignature]->AddEntity(entityId, newSignature, oldComponents);

        entitySignatureMap[entityId] = newSignature;
    }

    template <typename T>
    std::vector<Chunk *> GetEntities(std::bitset<MAX_COMPONENTS> mask)
    {
        std::vector<Chunk *> res;

        for (auto it : archMap)
        {
            auto key = it.first;
            key = key & mask;
            if (key == mask)
            {
                auto chunks = it.second->GetAllChunks();
                res.insert(res.end(), chunks.begin(), chunks.end());
            }
        }

        return res;
    }
};