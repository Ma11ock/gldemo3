#ifndef BINDABLE_HPP
#define BINDABLE_HPP

#include <cstdint>
#include <string>
#include <string_view>
#include <exception>
#include <stdexcept>
#include <fmt/core.h>

#include "glutil.hpp"

class Bindable
{
    friend class Drawable;
public:
    Bindable() : mId(0)
    {
    }
    virtual ~Bindable() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;
protected:
    std::uint32_t getID(const Bindable *bindable) const
    {
        return bindable->mId;
    }
    std::uint32_t mId;
};

#endif /* BINDABLE_HPP */
