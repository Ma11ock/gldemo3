#include "event.hpp"
#include <cctype>


bool proj::isCharKey(std::uint32_t code)
{
    return std::isgraph(static_cast<int>(code));
}

