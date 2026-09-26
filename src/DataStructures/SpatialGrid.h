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

    void Add(float x, float y, float radius, uint32_t id)
    {
        x += width / 2.0f;
        y += height / 2.0f;

        float minX = x - radius;
        float maxX = x + radius;
        float minY = y - radius;
        float maxY = y + radius;

        int gridMinX = static_cast<int>(minX / cellSize);
        int gridMaxX = static_cast<int>(maxX / cellSize);
        int gridMinY = static_cast<int>(minY / cellSize);
        int gridMaxY = static_cast<int>(maxY / cellSize);

        for (int i = gridMinY; i <= gridMaxY; i++)
        {
            for (int j = gridMinX; j <= gridMaxX; j++)
            {
                if (j >= 0 && j < columns && i >= 0 && i < rows)
                {
                    cells[i * columns + j].push_back(id);
                }
            }
        }
    }
};
