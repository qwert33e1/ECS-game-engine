#include "Game/Systems/HostileFollowSystem.h"

void HostileFollowSystem::Update(EntityManager &entityManager)
{
    // getting the first player controlled entity and its position
    uint32_t posId = ComponentRegistry::GetId<Position>();
    uint32_t pcId = ComponentRegistry::GetId<PlayerControlled>();

    uint32_t posSize = ComponentRegistry::globalSizeTable[posId];

    std::bitset<MAX_COMPONENTS> playerMask;

    playerMask.set(posId, true);
    playerMask.set(pcId, true);

    auto playerChunk = (entityManager.GetEntities(playerMask)).front();
    if (playerChunk->entityCounter == 0)
    {
        return;
    }

    uint32_t posOffset = playerChunk->archetype->GetComponentOffset(posId);

    Position *pos = reinterpret_cast<Position *>(playerChunk->data + posOffset);

    float pX = pos->x;
    float pY = pos->y;

    // getting all the entities with hostile tag and target component
    uint32_t hostileId = ComponentRegistry::GetId<Hostile>();
    uint32_t targetId = ComponentRegistry::GetId<TargetPosition>();

    uint32_t targetSize = ComponentRegistry::globalSizeTable[targetId];

    std::bitset<MAX_COMPONENTS> hostileMask;

    hostileMask.set(hostileId, true);
    hostileMask.set(targetId, true);

    std::vector<Chunk *> hostileChunks = entityManager.GetEntities(hostileMask);

    for (auto chunk : hostileChunks)
    {
        uint32_t posOffset = chunk->archetype->GetComponentOffset(posId);
        uint32_t targetOffset = chunk->archetype->GetComponentOffset(targetId);
        uint32_t entityCount = chunk->entityCounter;

        for (uint32_t i = 0; i < entityCount; i++)
        {
            TargetPosition *target = reinterpret_cast<TargetPosition *>(chunk->data + targetOffset + i * targetSize);

            target->x = pX;
            target->y = pY;
        }
    }
}