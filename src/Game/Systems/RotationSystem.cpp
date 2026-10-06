#include "Game/Systems/RotationSystem.h"

void RotationSystem::Update(EntityManager &entityManager)
{
    uint32_t rotId = ComponentRegistry::GetId<Rotation>();
    uint32_t velId = ComponentRegistry::GetId<Velocity>();
    uint32_t projId = ComponentRegistry::GetId<Projectile>();

    uint32_t rotSize = ComponentRegistry::globalSizeTable[rotId];
    uint32_t velSize = ComponentRegistry::globalSizeTable[velId];

    std::bitset<MAX_COMPONENTS> bitmask;

    bitmask.set(rotId, true);
    bitmask.set(velId, true);
    bitmask.set(projId, true);

    std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

    for (auto chunk : entityChunks)
    {
        uint32_t rotOffset = chunk->archetype->GetComponentOffset(rotId);
        uint32_t velOffset = chunk->archetype->GetComponentOffset(velId);
        uint32_t entityCount = chunk->entityCounter;

        for (uint32_t i = 0; i < entityCount; i++)
        {
            Rotation *rot = reinterpret_cast<Rotation *>(chunk->data + rotOffset + i * rotSize);
            Velocity *vel = reinterpret_cast<Velocity *>(chunk->data + velOffset + i * velSize);

            rot->angle = atan2(vel->y, vel->x);
        }
    }
}