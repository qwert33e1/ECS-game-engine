#pragma once
#include "common.h"

namespace ComponentRegistry
{
    inline uint32_t count;
    inline uint32_t globalSizeTable[MAX_COMPONENTS];

    template <typename T>
    uint32_t RegisterAndGetId()
    {
        globalSizeTable[count] = sizeof(T);
        return count++;
    }

    template <typename T>
    uint32_t GetId()
    {
        static uint32_t id = RegisterAndGetId<T>();
        return id;
    }
}