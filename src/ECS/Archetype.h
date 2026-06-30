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
    std::vector<uint8_t> componentArray;
    uint32_t maxEntityInChunk;
    uint32_t offsetMap[MAX_COMPONENTS];

public:
    Archetype(std::bitset<MAX_COMPONENTS> _signature)
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
};