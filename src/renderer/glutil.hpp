#ifndef GLUTIL_HPP
#define GLUTIL_HPP

#include <string_view>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <fmt/core.h>
#include <exception>
#include <stdexcept>

#ifdef DEBUG

inline int curLine = 0;
inline std::string_view curFile;
inline std::string_view curGlCommand;


#define GLCall(line) line;                      \
    curLine = __LINE__;                         \
    curGlCommand = #line;                       \
    curFile = __FILE__; 

#else
#define GLCall(line) line;
#endif // DEBUG

struct buffers
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texUVs;
    std::vector<glm::vec3> normals;
    std::vector<std::uint32_t> indices;

    static buffers createFromData(const std::vector<glm::vec3> &verts,
                                  const std::vector<glm::vec2> &uvs,
                                  const std::vector<glm::vec3> &normals);
};

struct interleavedType
{
    glm::vec3 vertexCoords;
    glm::vec2 texCoords;
    glm::vec3 normalCoords;
};


struct interleavedBuffers
{
    std::vector<interleavedType> interleavedBufs;
    std::vector<std::uint32_t> indexBuf;
};

class OpenGLException : public std::exception
{
public:
    OpenGLException(std::string_view what, std::string_view source,
                    std::string_view type)
        : mWhat(fmt::format("{} error in {}: \"{}\"", type, source, what)) 
    {
    }

    OpenGLException(std::string_view what,  std::string_view source,
                    std::string_view type, std::string_view command,
                    std::string_view file, int line)
        : mWhat(fmt::format("{} error in {} ({}:{} \"{}\"): {}",
                            type, source, file, line, command, what))
    {
    }

    const char *what() const throw()
    {
        return mWhat.c_str();
    }

    virtual ~OpenGLException() = default;

private:
    std::string mWhat;
};

inline interleavedBuffers bufsToInterleaved(const buffers &bufs)
{
    std::vector<interleavedType> overallBuffer(bufs.vertices.size());

    for(std::size_t i = 0; i < bufs.vertices.size(); i++)
        overallBuffer[i] = { bufs.vertices[i], bufs.texUVs[i],
            bufs.normals[i] };

    return interleavedBuffers{ overallBuffer, bufs.indices };
}


#endif /* GLUTIL_HPP */
