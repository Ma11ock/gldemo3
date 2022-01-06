
extern "C" {
#include <glad/glad.h>
#include <SDL.h>
    // TODO transition to glad.
#include <SDL_opengl.h>
}

#include "glutil.hpp"

#include "renderer.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "loadobj.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>
#include <string>
#include <fmt/core.h>
#include <chrono>

using namespace std::string_literals;

namespace
{
    void openGLMessageCallback(GLenum source, GLenum type, GLuint id,
                               GLenum severity, GLsizei length,
                               const GLchar *message,
                               const GLvoid *userParam)
    {
        auto errSrc = [source]()
        {
            switch (source)
            {
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: return "OTHER";
            default: return "Unknown";
            }
        };

        auto errType = [type]()
        {
            switch (type)
            {
            case GL_DEBUG_TYPE_ERROR: return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "MARKER";
            case GL_DEBUG_TYPE_OTHER: return "OTHER";
            default: return "Unknown";
            }
        };


        std::string_view severityStr;
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
#ifdef DEBUG

            std::cout << "In " << curFile << ':' << curLine << ": "
                      << curGlCommand << ": ";
    
#endif // DEBUG
            std::cout << errSrc() << ", " << errType() << ", Notification, "
                      << id << ": " << message << '\n';
            return;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severityStr = "LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severityStr = "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
#ifdef DEBUG
            throw OpenGLException(message, errSrc(), errType(),
                                  curGlCommand, curFile, curLine);
#else
            throw OpenGLException(message, errSrc(), errType());
#endif // DEBUG
            break;
        default:
            severityStr = "Unknown";
            break;
        }
#ifdef DEBUG

        std::cerr << "In " << curFile << ':' << curLine << ": "
                  << curGlCommand << ": ";
    
#endif // DEBUG
        std::cerr << errSrc() << ", " << errType() << ", " << severityStr 
                  << ", " << id << ": " << message << '\n';

    }

    // SDL window.
    SDL_Window *window = nullptr;
    // SDLGL context.
    SDL_GLContext context = {};

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
    Texture::init();
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

    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        throw std::runtime_error(fmt::format("Could not init glad"));


    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGLMessageCallback, nullptr);

    // Use vsync.
    if(SDL_GL_SetSwapInterval(1) < 0)
        std::cerr << "Warning: unable to use vsync: " << SDL_GetError() << '\n';

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);    
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
    SDL_GL_SwapWindow(window);
}


