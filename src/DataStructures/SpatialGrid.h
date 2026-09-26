#pragma once

#include <vector>
#include <cstdint>

class SpatialGrid
{
    float width;
    float height;
    int columns;
    int rows;
    float cellSize;

    std::vector<std::vector<uint32_t>> cells;

public:
    SpatialGrid(float cellSize, float width, float height) : cellSize(cellSize), width(width), height(height)
    {
        this->columns = width / cellSize;
        this->rows = height / cellSize;

        cells.resize(this->columns * this->rows);
    }

    void Add(float x, float y, uint32_t id)
    {
        x += width / 2.0f;
        y += height / 2.0f;

        int gridX = static_cast<int>(x / cellSize);
        int gridY = static_cast<int>(y / cellSize);

        if (gridX < 0 || gridX >= columns || gridY < 0 || gridY >= rows)
        {
            return;
        }

        cells[gridY * columns + gridX].push_back(id);
    }
};
