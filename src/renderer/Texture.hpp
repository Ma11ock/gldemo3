#ifndef PROJ_TEXTURE_HPP
#define PROJ_TEXTURE_HPP

#include "Bindable.hpp"

#include <cstdint>
#include <filesystem>

class Texture : public Bindable
{
public:
    Texture()
        : Bindable(),mWidth(0),mHeight(0),mNumChannels(0),mBitsPerPixel(0)
    {
    }

    Texture(const std::filesystem::path &path, int numChannels = 4);

    virtual void bind();
    virtual void unbind();
    virtual ~Texture() = default;

    static void init();
private:
    int mWidth;
    int mHeight;
    int mNumChannels;
    int mBitsPerPixel;
};

#endif /* PROJ_TEXTURE_HPP */
