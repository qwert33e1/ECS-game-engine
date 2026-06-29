#pragma once
#include <cstdint>

namespace ComponentRegistry
{
    uint32_t count;

    template <typename T>
    uint32_t GetId()
    {
        static uint32_t id = count++;
        return id;
    }
}