#include "ComponentRegistry.h"

uint32_t ComponentRegistry::count = 0;

template <typename T>
uint32_t ComponentRegistry::GetId<T>()
{
    static uint32_t id = count;
    count++;
    return id;
}