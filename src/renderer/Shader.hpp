#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <filesystem>
#include <cstdint>
#include <utility>

#include "Bindable.hpp"

class Shader : public Bindable
{
public:
    static inline constexpr std::array SHADER_TYPES = {
        std::make_pair(".vert", GL_VERTEX_SHADER),
        std::make_pair(".vs", GL_VERTEX_SHADER),
        std::make_pair(".frag", GL_FRAGMENT_SHADER),
        std::make_pair(".fs", GL_FRAGMENT_SHADER),
        std::make_pair(".tesc", GL_TESS_CONTROL_SHADER),
        std::make_pair(".tsc", GL_TESS_CONTROL_SHADER),
        std::make_pair(".tese", GL_TESS_EVALUATION_SHADER),
        std::make_pair(".tes", GL_TESS_EVALUATION_SHADER),
        std::make_pair(".comp", GL_COMPUTE_SHADER),
        std::make_pair(".cs", GL_COMPUTE_SHADER),
        std::make_pair(".geom", GL_GEOMETRY_SHADER),
        std::make_pair(".gs", GL_GEOMETRY_SHADER),
    };

    Shader() : mId(0)
    {
    }

    template<typename ... Args>
    Shader(Args &&... args)
    {
        std::array unpack = {compileShader(std::forward<Args>(args))...};
        mId = GLCall(glCreateProgram());
        std::cout << "Compiling shaders.\n";
        for(auto i : unpack)
            GLCall(glAttachShader(mId, i));
        std::cout << "Linking shaders.\n";
        GLCall(glLinkProgram(mId));
        std::cout << "Deleting shaders.\n";
        for(auto i : unpack)
            GLCall(glDeleteShader(i));
        std::cout << "Done creating shader program.\n";
    }

    // Compile shader, return OpenGL ID of shader.
    std::uint32_t compileShader(const std::filesystem::path &path);
    // Get the type of shader from its file path.
    GLenum getShaderType(const std::filesystem::path &path);

    // activate the shader
    virtual void bind() 
    { 
        glUseProgram(mId); 
    }
    // Deactivate the shader.
    virtual void unbind()
    {
        glUseProgram(0); 
    }
    // utility uniform functions
    inline void set(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(mId, name.c_str()),
                    static_cast<std::int32_t>(value)); 
    }
    inline void set(const std::string &name, std::int32_t value) const
    { 
        glUniform1i(glGetUniformLocation(mId, name.c_str()), value); 
    }
    inline void set(const std::string &name, std::uint32_t value) const
    { 
        glUniform1ui(glGetUniformLocation(mId, name.c_str()), value); 
    }
    inline void set(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(mId, name.c_str()), value); 
    }
    inline void set(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(mId, name.c_str()), 1,
                     glm::value_ptr(value)); 
    }
    inline void set(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(mId, name.c_str()), x, y); 
    }
    inline void set(const std::string &name,
                    const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(mId, name.c_str()), 1,
                     glm::value_ptr(value)); 
    }
    inline void set(const std::string &name, float x, float y,
                    float z) const
    { 
        glUniform3f(glGetUniformLocation(mId, name.c_str()), x, y, z); 
    }
    inline void set(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(mId, name.c_str()), 1,
                     glm::value_ptr(value)); 
    }
    inline void set(const std::string &name, float x, float y, float z,
                    float w) 
    { 
        glUniform4f(glGetUniformLocation(mId, name.c_str()), x, y, z, w); 
    }
    inline void set(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(mId, name.c_str()), 1,
                           GL_FALSE, glm::value_ptr(mat));
    }
    inline void set(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(mId, name.c_str()), 1,
                           GL_FALSE, glm::value_ptr(mat));
    }
    inline void set(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(mId, name.c_str()), 1,
                           GL_FALSE, glm::value_ptr(mat));
    }

private:
    std::uint32_t mId;
};
#endif // SHADER_HPP
