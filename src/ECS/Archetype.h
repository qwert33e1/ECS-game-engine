#pragma once

#include "common.h"
#include "ComponentRegistry.h"
#include <limits>

#define CHUNK_SIZE 4096

class Archetype;

struct Chunk
{
    Archetype *arcehtype;
    uint32_t entityCounter;

    /// First maxEntityInChunk * sizeof(uint32_t) byte is lookup table for entity IDs
    uint8_t data[CHUNK_SIZE];
};

class Archetype
{
private:
    std::bitset<MAX_COMPONENTS> signature;
    std::vector<std::unique_ptr<Chunk>> componentChunks;
    std::vector<std::unique_ptr<Chunk>> chunkPool;
    std::unordered_map<uint32_t, Chunk *> entityChunkMap;
    uint32_t maxEntityInChunk;
    uint32_t componentOffsetMap[MAX_COMPONENTS];

public:
    Archetype(std::bitset<MAX_COMPONENTS> _signature, std::vector<std::unique_ptr<Chunk>>)
    {
        signature = _signature;

        uint32_t entitySize = 0;

        for (size_t i = 0; i < signature.size(); i++)
        {
            if (signature[i])
            {
                entitySize += ComponentRegistry::globalSizeTable[i];
            }
        }

        entitySize += sizeof(uint32_t); // space for the lookup table entry

        maxEntityInChunk = CHUNK_SIZE / entitySize;
        uint32_t indexEntityMapSize = maxEntityInChunk * sizeof(uint32_t);
        uint32_t currentOffset = indexEntityMapSize;

        for (size_t i = 0; i < signature.size(); i++)
        {
            if (signature[i])
            {
                componentOffsetMap[i] = currentOffset;
                currentOffset += maxEntityInChunk * ComponentRegistry::globalSizeTable[i];
            }
            else
            {
                componentOffsetMap[i] = 0; // 0 is the invalid entry (0 is always the offset of indexEntityMap, and it is not stored in the array)
            }
        }
    }

    template <typename... Ts>
    void AddEntity(uint32_t entityId, Ts &&...components)
    {
        Chunk *currentChunk = GetCurrentChunk();
        (PlaceData<Ts>(currentChunk, std::forward<Ts>(components)...), ...);

        uint32_t *indexEntityMap = reinterpret_cast<uint32_t *>(currentChunk->data);
        indexEntityMap[currentChunk->entityCounter] = entityId;

        currentChunk->entityCounter++;

        entityChunkMap[entityId] = currentChunk;
    }

    template <typename T>
    void PlaceData(Chunk *chunk, const T &data)
    {
        uint32_t offset = componentOffsetMap[ComponentRegistry::GetId<T>()];
        T *dest = reinterpret_cast<T *>(chunk->data + offset);
        dest[chunk->entityCounter] = data;
    }

    /// returns the last chunk of the componentChunks vector if it is able to place a new entry
    /// if the last chunk is full, this method appends an empty chunk to the vector and return that
    Chunk *GetCurrentChunk()
    {
        if (!componentChunks.empty())
        {
            Chunk *back = componentChunks.back().get();
            if (back->entityCounter < maxEntityInChunk)
            {
                return back;
            }
        }
        if (!chunkPool.empty())
        {
            componentChunks.push_back(std::move(chunkPool.back()));
            chunkPool.pop_back();
        }
        else
        {
            componentChunks.push_back(std::make_unique<Chunk>());
        }

        Chunk *newChunk = componentChunks.back().get();
        newChunk->arcehtype = this;
        newChunk->entityCounter = 0;

        return newChunk;
    }

    void RemoveEntity(uint32_t entityId)
    {
        if (!entityChunkMap.contains(entityId))
        {
            return;
        }

        Chunk *chunk = entityChunkMap[entityId];
        uint32_t *indexEntityMap = reinterpret_cast<uint32_t *>(chunk->data);

        uint32_t indexInChunk = std::numeric_limits<uint32_t>::max();
        for (uint32_t i = 0; i < maxEntityInChunk; i++)
        {
            if (indexEntityMap[i] == entityId)
            {
                indexInChunk = i;
                break;
            }
        }

        if (indexInChunk == std::numeric_limits<uint32_t>::max())
        {
            return;
        }

        SwapAndPop(chunk, indexInChunk, entityId);
    }

    void SwapAndPop(Chunk *targetChunk, uint32_t targetIndex, uint32_t targetId)
    {
        if (componentChunks.empty())
        {
            return;
        }

        Chunk *lastChunk = componentChunks.back().get();
        /// TODO: this should not happend, but needs better exception handling
        if (lastChunk->entityCounter == 0)
        {
            return;
        }

        uint32_t lastIndex = lastChunk->entityCounter - 1;
        uint32_t *lastIndexEntityMap = reinterpret_cast<uint32_t *>(lastChunk->data);
        uint32_t lastEntityId = lastIndexEntityMap[lastIndex];

        /// if the targeted entity is the last entry, than no swap needed, we just decrease the counter
        if (!(lastChunk == targetChunk && lastIndex == targetIndex))
        {
            uint32_t *TargetIndexEntityMap = reinterpret_cast<uint32_t *>(targetChunk->data);
            TargetIndexEntityMap[targetIndex] = lastEntityId;
            entityChunkMap[lastEntityId] = targetChunk;

            for (size_t i = 0; i < signature.size(); i++)
            {
                uint32_t offset = componentOffsetMap[i];
                if (offset != 0)
                {
                    uint32_t size = ComponentRegistry::globalSizeTable[i];

                    std::memcpy(targetChunk->data + offset + targetIndex * size, lastChunk->data + offset + lastIndex * size, size);
                }
            }
        }

        entityChunkMap.erase(targetId);

        lastChunk->entityCounter--;

        if (lastChunk->entityCounter == 0)
        {
            chunkPool.push_back(std::move(componentChunks.back()));
            componentChunks.pop_back();
        }
    }
};