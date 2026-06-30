#pragma once
#include "common.h"

namespace ComponentRegistry
{
    uint32_t count;
    uint32_t globalSizeTable[MAX_COMPONENTS];

    template <typename T>
    uint32_t GetId()
    {
        static uint32_t id = RegisterAndGetId<T>();
        return id;
    }

    template <typename T>
    uint32_t RegisterAndGetId()
    {
        globalSizeTable[count] = sizeof(T);
        return count++;
    }
}