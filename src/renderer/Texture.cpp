#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION 1
extern "C" {
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <stb/stb_image.h>
}

#include <filesystem>
#include <iostream>
#include <fmt/core.h>

namespace fs = std::filesystem;

void Texture::init()
{
    std::cout << "Initializing STB image.\n";
    stbi_set_flip_vertically_on_load(true); 
    std::cout << "Done initializing STB image.\n";
}

Texture::Texture(const fs::path &path, int numChannels)
    : Texture()
{
    std::uint8_t *ptr = stbi_load(path.c_str(), &mWidth, &mHeight,
                                  &mNumChannels, numChannels);
    
    if(!ptr)
        throw std::invalid_argument(fmt::format("Could not open texture at {}",
                                                path.generic_string()));

    
    glCreateTextures(GL_TEXTURE_2D, 1, &mId);
    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(mId, 1, GL_RGB8, mWidth, mHeight);
    glTextureSubImage2D(mId, 0, 0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, ptr);

    stbi_image_free(ptr);
}


void Texture::bind()
{
    glBindTextureUnit(0, mId);
}

void Texture::unbind()
{
    
}
