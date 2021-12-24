#ifndef GLUTIL_HPP
#define GLUTIL_HPP

#include <string_view>
#include <string>
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


#endif /* GLUTIL_HPP */
