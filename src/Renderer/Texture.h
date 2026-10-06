#pragma once

#include <glad/glad.h>
#include "picoPNG.h"

class Texture
{
    unsigned int textureId;
    unsigned long w, h;

public:
    Texture(const char *pathname, int sampling = GL_LINEAR);

    unsigned int GetId()
    {
        return textureId;
    }

    unsigned long GetWidth() { return w; }

    unsigned long GetHeight() { return h; }

    ~Texture()
    {
        glDeleteTextures(1, &textureId);
    }
};