#include "glutil.hpp"

#define STB_IMAGE_IMPLEMENTATION 1

extern "C" {
#include <SDL2/SDL.h>
    // TODO transition to glad.
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <stb/stb_image.h>
}

#include "renderer.hpp"
#include "shader.hpp"
#include "Camera.hpp"
#include "matrix.hpp"
#include "loadobj.hpp"
#include "VertexArray.hpp"

#include <glm/glm.hpp>
#include <string>
#include <fmt/core.h>
#include <chrono>

using namespace std::string_literals;

struct thing
{
    buffers bufs;
    GLuint texId = 0;
    std::unique_ptr<VertexArray> va;

    void draw(const Shader &shader)
    {
        ms::pushMatricesToShaders(shader);
        glBindTextureUnit(0, texId);
        va->draw();
    }

    void createThing(const std::filesystem::path &objPath,
                     const std::filesystem::path &texPath)
    {

        int texWidth = 0;
        int texHeight = 0;
        int nrChans = 0;
        std::uint8_t *ptr = stbi_load(texPath.c_str(), &texWidth, &texHeight,
                                      &nrChans, 3);
        if(!ptr)
            throw std::invalid_argument("Could not open texture at "s + texPath.generic_string());

        glCreateTextures(GL_TEXTURE_2D, 1, &texId);
        glTextureParameteri(texId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(texId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureStorage2D(texId, 1, GL_RGB8, texWidth, texHeight);
        glTextureSubImage2D(texId, 0, 0, 0, texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, ptr);

        stbi_image_free(ptr);

        // OBJ file.
        bufs = loadObjFile(objPath);
        std::vector<interleavedType> overallBuffer(bufs.vertices.size());

        for(std::size_t i = 0; i < bufs.vertices.size(); i++)
            overallBuffer[i] = { bufs.vertices[i], bufs.texUVs[i],
                bufs.normals[i] };

        va = std::make_unique<VertexArray>(overallBuffer, bufs.indices);
    }
};

namespace
{
    thing claire;
    thing tyrant;
    thing leon;
    thing teapot;
    Camera camera(glm::vec3(30.f, 30.f, 30.f));
    // SDL window.
    SDL_Window *window = nullptr;
    // SDLGL context.
    SDL_GLContext context = {};

    Shader ourShader;
    float scrWidth = 0.f;
    float scrHeight = 0.f;
    float lastX = 0.f;
    float lastY = 0.f;
    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
}

void rndr::init(const std::string &title, int width, int height)
{
    stbi_set_flip_vertically_on_load(true); 
    if(auto ret = SDL_Init(SDL_INIT_EVERYTHING);
       ret != 0)
        throw std::runtime_error(
            fmt::format("Could not initialize SDL. Got return code {} and message \"{}\"",
                        ret, SDL_GetError()));

    scrWidth = static_cast<float>(width);
    scrHeight = static_cast<float>(height);
    lastX = scrWidth / 2.f;
    lastY = scrHeight / 2.f;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_OPENGL);
    if(!window)
        throw std::runtime_error("Could not create window: "s + SDL_GetError());

    context = SDL_GL_CreateContext(window);
    if(!context)
        throw std::runtime_error("Could not create OpenGL context: "s + SDL_GetError());

    {
        int dubBuf = 0;
        SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &dubBuf);
        if(dubBuf)
            std::cout << "Doublebuffeering enabled.\n";
        else
            std::cerr << "Doublebuffering could not be enabled.\n";
        // TODO check other OpenGL values.
    }


    // Initialize glew.
    glewExperimental = GL_TRUE;
    auto glewError = glewInit();
    if(glewError != GLEW_OK)
        throw std::runtime_error(fmt::format("Could not initialize glew: {}",
                                             glewGetErrorString(glewError)));

    // Use vsync.
    if(SDL_GL_SetSwapInterval(1) < 0)
        std::cerr << "Warning: unable to use vsync: " << SDL_GetError() << '\n';

    ourShader = Shader("main.vert", "main.frag");

    claire.createThing("claire.obj", "claire.bmp");
    tyrant.createThing("tyrant.obj", "tyrant.bmp");
    leon.createThing("leon.obj", "leon.bmp");
    teapot.createThing("teapot.obj", "earth.jpg");

}

void rndr::quit()
{
    std::cout << "Quitting the graphics system.\n";
    if(window)
    {
        SDL_GL_DeleteContext(context);
        std::cout << "Killing the window.\n";
        SDL_DestroyWindow(window);
        std::cout << "Killing SDL.\n";
        SDL_Quit();
    }
    std::cout << "Done quitting the graphics system.\n";
}


void rndr::clearWindow()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void rndr::present()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);    
    ourShader.use();

    
    ms::setMatrixMode(ms::Stack::Projection, true);
    ms::perspective(glm::radians(camera.Zoom),
                    1.f,
                    0.1f,
                    1000.f);

    ms::setMatrixMode(ms::Stack::View, true);
    ms::loadMatrix(camera.GetViewMatrix());

    glm::vec3 lightColor(1.f, 1.f, 1.f);

    ourShader.set("uViewPos", camera.Position);
    ourShader.set("uNumDirLights", 1u);
    ourShader.set("uNumPointLights", 0u);
    ourShader.set("uNumSpotLights", 0u);
    ourShader.set("uDirLights[0].specular", lightColor * glm::vec3(1.f));
    ourShader.set("uDirLights[0].diffuse", lightColor * glm::vec3(1.f));
    ourShader.set("uDirLights[0].ambient", lightColor * glm::vec3(0.2f));
    ourShader.set("uDirLights[0].direction", -0.2f, -1.f, -0.3f);
    ourShader.set("uMaterial.diffuse", 0);
    ourShader.set("uMaterial.specular", 0);
    ourShader.set("uMaterial.shininess", 64.f);

    ms::setMatrixMode(ms::Stack::Model, true);
    ms::translate(10.f, 80.f, 20.f);
    teapot.draw(ourShader);
    ms::loadIdentity();
    ms::translate(glm::vec3(20.f, 0.f, 10.f));

    ms::rotate(glm::sin(std::chrono::system_clock::now().time_since_epoch().count() / 1000000),
               glm::vec3(0.f, 1.f, 0.f));
    claire.draw(ourShader);

    ms::loadIdentity();
    ms::translate(glm::vec3(15.f, 0.f, 45.f));
    tyrant.draw(ourShader);

    ms::loadIdentity();
    ms::translate(glm::vec3(-10.f, 0.f, 20.f));
    ms::rotate(glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
    leon.draw(ourShader);

    SDL_GL_SwapWindow(window);
}
