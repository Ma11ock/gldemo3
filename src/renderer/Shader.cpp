#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <fmt/core.h>
#include "../util.hpp"

namespace fs = std::filesystem;

std::uint32_t Shader::compileShader(const fs::path &path)
{
    std::uint32_t shader = 0;

    auto type = getShaderType(path);
    if(type == 0)
        return 0;

    std::ifstream shaderCode;
    std::string shaderString;
    shaderCode.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderCode.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderCode.rdbuf();
        shaderString = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        throw std::runtime_error(fmt::format("Unable to read file {}: {}",
                                             path, e.what()));
    }

    const char *shaderCString = shaderString.c_str();
    shader = GLCall(glCreateShader(type));
    GLCall(glShaderSource(shader, 1, &shaderCString, nullptr));
    GLCall(glCompileShader(shader));

    return shader;
}
