#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "Bindable.hpp"
#include "IndexBuffer.hpp"
#include "ConstantBuffer.hpp"
#include "loadobj.hpp"

#include <memory>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <glm/glm.hpp>

class VertexArray : public Bindable
{
public:
    VertexArray(const std::vector<glm::vec3> &vertices,
                const std::vector<glm::vec2> &texCoords,
                const std::vector<glm::vec3> &normals,
                const std::vector<std::uint32_t> &indices) : Bindable()
    {
        GLCall(glCreateVertexArrays(1, &mId));
        mVertexBuffer = std::make_unique<ConstantBuffer>(vertices, 0, mId);
        mTextureBuffer = std::make_unique<ConstantBuffer>(texCoords, 1, mId);
        mNormalBuffer = std::make_unique<ConstantBuffer>(normals, 2, mId);
        mIndexBuffer = std::make_unique<IndexBuffer>(indices);

        GLCall(glVertexArrayElementBuffer(mId,
                                          getID(mIndexBuffer.get())));

    }

    VertexArray(const std::vector<interleavedType> &vertices,
                const std::vector<std::uint32_t> &indices) : Bindable()
    {
        GLCall(glCreateVertexArrays(1, &mId));

        mVertexBuffer = std::make_unique<ConstantBuffer>(vertices, 0, mId);
        mIndexBuffer = std::make_unique<IndexBuffer>(indices);
        GLCall(glVertexArrayElementBuffer(mId,
                                          getID(mIndexBuffer.get())));
    }

    VertexArray(const interleavedBuffers &ibs)
        : VertexArray(ibs.interleavedBufs, ibs.indexBuf)
    {
    }

    VertexArray(const buffers &bufs)
        : VertexArray(bufs.vertices, bufs.texUVs, bufs.normals,
                      bufs.indices)
    {
    }

    VertexArray(const std::filesystem::path &path)
    //: VertexArray(bufsToInterleaved(loadObjFile(path)))
        : VertexArray(loadObjFile(path))
    {
    }

    VertexArray() = default;

    virtual void bind() 
    {
        GLCall(glBindVertexArray(mId));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                            getID(mIndexBuffer.get())));
        GLCall(glDrawElements(GL_TRIANGLES, mIndexBuffer->getNumIndices(),
               GL_UNSIGNED_INT, nullptr));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        GLCall(glBindVertexArray(0));
    }

    virtual void unbind()
    {
    }

    virtual ~VertexArray() = default;
protected:

    std::unique_ptr<ConstantBuffer> mVertexBuffer;
    std::unique_ptr<ConstantBuffer> mTextureBuffer;
    std::unique_ptr<ConstantBuffer> mNormalBuffer;
    std::unique_ptr<IndexBuffer> mIndexBuffer;
};

#endif /* VERTEX_ARRAY_HPP */
