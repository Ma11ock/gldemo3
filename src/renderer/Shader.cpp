#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <filesystem>

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
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << '\n';
        return 0;
    }

    const char *shaderCString = shaderString.c_str();
    shader = GLCall(glCreateShader(type));
    GLCall(glShaderSource(shader, 1, &shaderCString, nullptr));
    GLCall(glCompileShader(shader));

    return shader;
}
