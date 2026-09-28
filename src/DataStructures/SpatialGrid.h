#pragma once

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

struct FatCell
{
    uint32_t id;
    float x;
    float y;
    float radius;
};

class SpatialGrid
{
    float mWidth;
    float mHeight;
    int gridWidth;
    int gridHeight;
    float cellSize;

    std::vector<std::vector<FatCell>> cells;
    std::vector<glm::ivec2> activeCells;

public:
    SpatialGrid(float cellSize, float width, float height) : cellSize(cellSize), mWidth(width), mHeight(height)
    {
        this->gridWidth = static_cast<int>(mWidth / cellSize) + 1;
        this->gridHeight = static_cast<int>(mHeight / cellSize) + 1;

        cells.resize(this->gridWidth * this->gridHeight);
    }

    void Add(float x, float y, float radius, uint32_t id)
    {
        x += mWidth / 2.0f;
        y += mHeight / 2.0f;

        int gridX = static_cast<int>(x / cellSize);
        int gridY = static_cast<int>(y / cellSize);

        if (gridX < 0 || gridX >= gridWidth || gridY < 0 || gridY >= gridHeight)
        {
            return;
        }

        size_t index = gridY * gridWidth + gridX;

        if (cells[index].empty())
        {
            activeCells.push_back(glm::ivec2{gridX, gridY});
        }

        cells[index].push_back(FatCell{id, x, y, radius});
    }

    void Clear()
    {
        for (auto const &cell : activeCells)
        {
            int flatIndex = cell.y * gridWidth + cell.x;
            cells[flatIndex].clear();
        }
        activeCells.clear();
    }

    std::vector<std::vector<FatCell>> &GetCells()
    {
        return cells;
    }

    std::vector<glm::ivec2> &GetActiveCells()
    {
        return activeCells;
    }

    int getGridWidth()
    {
        return gridWidth;
    }

    int getGridHeight()
    {
        return gridHeight;
    }
};
