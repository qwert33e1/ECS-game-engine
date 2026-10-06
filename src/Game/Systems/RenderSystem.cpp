#include "Game/Systems/RenderSystem.h"

void RenderSystem::Update(EntityManager &entityManager)
{
    // getting the camera entity
    uint32_t cameraId = ComponentRegistry::GetId<Camera>();
    uint32_t cameraSize = ComponentRegistry::globalSizeTable[cameraId];
    std::bitset<MAX_COMPONENTS> cameraMask;
    cameraMask.set(cameraId, true);
    auto cameraChunk = (entityManager.GetEntities(cameraMask)).front();
    if (cameraChunk->entityCounter == 0)
    {
        return;
    }
    uint32_t cameraOffset = cameraChunk->archetype->GetComponentOffset(cameraId);
    Camera *camera = reinterpret_cast<Camera *>(cameraChunk->data + cameraOffset);

    uint32_t posId = ComponentRegistry::GetId<Position>();
    uint32_t spriteId = ComponentRegistry::GetId<Sprite>();
    uint32_t rotId = ComponentRegistry::GetId<Rotation>();

    uint32_t posSize = ComponentRegistry::globalSizeTable[posId];
    uint32_t spriteSize = ComponentRegistry::globalSizeTable[spriteId];
    uint32_t rotSize = ComponentRegistry::globalSizeTable[rotId];

    std::bitset<MAX_COMPONENTS> bitmask;

    bitmask.set(posId, true);
    bitmask.set(spriteId, true);
    bitmask.set(rotId, true);

    std::vector<Chunk *> entityChunks = entityManager.GetEntities(bitmask);

    /// --- infinite scrolling background ---

    float tileSize = 64.0f;

    float uvOffsetX = fmodf(camera->x / tileSize, 1.0f);
    float uvOffsetY = fmodf(camera->y / tileSize, 1.0f);

    if (uvOffsetX < 0.0f)
    {
        uvOffsetX += 1.0f;
    }
    if (uvOffsetY < 0.0f)
    {
        uvOffsetY += 1.0f;
    }

    float spanX = WINDOW_WIDTH / tileSize;
    float spanY = WINDOW_HEIGHT / tileSize;

    glm::vec2 uvMin(uvOffsetX, uvOffsetY);
    glm::vec2 uvMax(uvOffsetX + spanX, uvOffsetY + spanY);

    renderer.AddQuad(glm::vec2((float)WINDOW_WIDTH / 2.0f, (float)WINDOW_HEIGHT / 2.0f), (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), background.GetId(), uvMin, uvMax);

    /// --- --- ---

    for (auto chunk : entityChunks)
    {
        uint32_t posOffset = chunk->archetype->GetComponentOffset(posId);
        uint32_t spriteOffset = chunk->archetype->GetComponentOffset(spriteId);
        uint32_t rotOffset = chunk->archetype->GetComponentOffset(rotId);
        uint32_t entityCount = chunk->entityCounter;

        for (uint32_t i = 0; i < entityCount; i++)
        {
            Position *pos = reinterpret_cast<Position *>(chunk->data + posOffset + i * posSize);
            Sprite *sprite = reinterpret_cast<Sprite *>(chunk->data + spriteOffset + i * spriteSize);
            Rotation *rot = reinterpret_cast<Rotation *>(chunk->data + rotOffset + i * rotSize);

            glm::vec2 sPos = Coordinates::WorldToScreen(pos->x, pos->y, *camera);

            SubTexture subTexture = spriteManager.GetSprite(sprite->spriteId);

            renderer.AddQuad(sPos, 64.0f, 64.0f, rot->angle, sprite->color, subTexture.GetId(), subTexture.GetUVMin(), subTexture.GetUVMax());
        }
    }
    renderer.updateGPU();
    renderer.Flush();
    renderer.Clear();
}

void RenderSystem::LoadTexture()
{
    spriteManager.LoadSheet("Textures/spritesheet.png", "Textures/spritesheet.json");
}