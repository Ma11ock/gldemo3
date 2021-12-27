#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <filesystem>

class VertexArray;
class Texture;
class Shader;

namespace graph
{
    void init(const std::string &windowTitle, int width, int height);
    void quit();

    bool createWindowMsg(const std::string &msg);
    bool createWindowWarning(const std::string &msg);
    bool createWindowError(const std::string &msg);
    void clearWindow();
    void present();

    void mouseMoved(float x, float y); 

    class Thing
    {
    public:
        Thing(const std::filesystem::path &objPath,
              const std::filesystem::path &texPath,
              std::shared_ptr<Shader> shader);
        Thing() = default;
        virtual ~Thing() = default;

        void draw();
        void translate(const glm::vec3 &xyz);
        void scale(const glm::vec3 &xyz);
        void rotate(float radAngle, const glm::vec3 &xyz);
    protected:
        std::shared_ptr<VertexArray> mVao;
        std::shared_ptr<Texture> mTexture;
        std::shared_ptr<Shader> mShader;
        glm::mat4 mTransforms;
    };
}

#endif /* GRAPHICS_HPP */
