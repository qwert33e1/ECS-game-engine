#pragma once

#include "common.h"
#include "ComponentRegistry.h"

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
    uint32_t maxEntityInChunk;
    uint32_t offsetMap[MAX_COMPONENTS];

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
                offsetMap[i] = currentOffset;
                currentOffset += maxEntityInChunk * ComponentRegistry::globalSizeTable[i];
            }
        }
    }

    template <typename... Ts>
    void AddEntity(uint32_t entityId, Ts &&...components)
    {
        Chunk *currentChunk = GetCurrentChunk();
        (PlaceData<Ts>(components, currentChunk), ...);

        uint32_t *indexEntityMap = reinterpret_cast<uint32_t *>(currentChunk->data);
        indexEntityMap[currentChunk->entityCounter] = entityId;

        currentChunk->entityCounter++;
    }

    template <typename T>
    void PlaceData(T data, Chunk *chunk)
    {
        uint32_t offset = offsetMap[ComponentRegistry::GetId<T>()];
        T *dest = reinterpret_cast<T *>(chunk->data + offset);
        dest[chunk->entityCounter] = data;
    }

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
        return componentChunks.back().get();
    }
};