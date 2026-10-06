#include "Texture.h"

Texture::Texture(const char *pathname, int sampling)
{
    // load and decode
    std::vector<unsigned char> buffer, image;
    loadFile(buffer, pathname);
    int error = decodePNG(image, w, h, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size());

    std::cout << "image size: " << image.size() << std::endl;

    // if there's an error, display it
    if (error != 0)
    {
        std::cout << "error: " << error << std::endl;
        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampling);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampling);
}