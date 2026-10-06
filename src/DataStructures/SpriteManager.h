#pragma once

#include <fstream>
#include <unordered_map>
#include <string>
#include <memory>
#include "SubTexture.h"
#include "Renderer/Texture.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class SpriteManager
{
    std::unordered_map<std::string, SubTexture> sprites;
    std::vector<std::shared_ptr<Texture>> loadedTextures;

public:
    void
    AddSprite(const std::string &name, const SubTexture &sprite)
    {
        sprites[name] = sprite;
    }

    SubTexture GetSprite(const std::string &name)
    {
        auto it = sprites.find(name);

        if (it != sprites.end())
        {
            return it->second;
        }

        std::cout << "! SpriteManager could not find: '" << name << "'\n";

        return SubTexture();
    }

    void Clear()
    {
        sprites.clear();
    }

    void LoadSheet(const char *texturePath, const char *jsonPath)
    {
        auto spriteSheet = std::make_shared<Texture>(texturePath);
        loadedTextures.push_back(spriteSheet);

        std::ifstream file(jsonPath);
        if (!file.is_open())
        {
            return;
        }
        json data = json::parse(file);

        for (auto &[spriteName, info] : data["frames"].items())
        {
            float x = info["frame"]["x"];
            float y = info["frame"]["y"];
            float w = info["frame"]["w"];
            float h = info["frame"]["h"];

            SubTexture subTexture(spriteSheet, x, y, w, h);
            sprites.emplace(spriteName, subTexture);
        }
    }
};