#include "graphics.hpp"

extern "C" {
#include <SDL.h>
}

#include <string>
#include "renderer/Texture.hpp"
#include "renderer/VertexArray.hpp"
#include "renderer/renderer.hpp"
#include "renderer/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void graph::init(const std::string &windowTitle, int width,
                 int height)
{
    rndr::init(windowTitle, width, height);
}

void graph::quit()
{
    rndr::quit();
}

bool graph::createWindowError(const std::string &msg)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                                    msg.c_str(),
                                    nullptr) == 0;
}

bool graph::createWindowWarning(const std::string &msg)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning",
                                    msg.c_str(),
                                    nullptr) == 0;
}

bool graph::createWindowMsg(const std::string &msg)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information",
                                    msg.c_str(),
                                    nullptr) == 0;
}

void graph::clearWindow()
{
    rndr::clearWindow();
}

void graph::present()
{
    rndr::present();
}

graph::Thing::Thing(const std::filesystem::path &objPath,
             const std::filesystem::path &texPath,
             std::shared_ptr<Shader> shader)
    : mVao(std::make_shared<VertexArray>(objPath)),
      mTexture(std::make_shared<Texture>(texPath)),
      mShader(shader),mTransforms(glm::mat4(1.f))
{
}

void graph::Thing::draw(const glm::mat4 &view, const glm::mat4 &projection)
{
    glm::mat4 modelViewMatrix = view * mTransforms;
    glm::mat4 modelViewProjectionMatrix = projection * modelViewMatrix;
    
    mShader->set("uModelViewMatrix", modelViewMatrix);
    mShader->set("uModelViewProjectionMatrix", modelViewProjectionMatrix);
    mShader->set("uModelMatrix", mTransforms);
    mShader->set("uNormalMatrix", glm::mat3(glm::transpose(glm::inverse(mTransforms))));
    mTexture->bind();
    mVao->bind();
}

void graph::Thing::translate(const glm::vec3 &xyz)
{
    mTransforms = glm::translate(mTransforms, xyz);
}

void graph::Thing::scale(const glm::vec3 &xyz)
{
    mTransforms = glm::scale(mTransforms, xyz);
}

void graph::Thing::rotate(float radAngle, const glm::vec3 &xyz)
{
    mTransforms = glm::rotate(mTransforms, radAngle, xyz);
}
