#ifndef LOADOBJ_H
#define LOADOBJ_H

#include <vector>
#include <tuple>
#include <cstdint>
#include <filesystem>
#include <glm/glm.hpp>
#include "glutil.hpp"

buffers loadObjFile(const std::filesystem::path &path);

#endif /* LOADOBJ_H */
