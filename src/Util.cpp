#include "glad/glad.h"
#include "Util.h"
#include "iostream"

Util::Util() {};

Util::~Util()
{
}

GLenum Util::CheckOpenGLError()
{
    GLenum errorCode;
    /*
     * OpenGL 维护一个错误队列。
     * glGetError() 不仅返回错误，还会从队列中移除该错误。
     * 循环调用直到返回 GL_NO_ERROR 可以确保错误队列被完全清空。
    */
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
        default:
            error = "UNKNOWN_ERROR";
            break;
        }
        std::cout << "code:" << errorCode << "\n" << error << " | " << __FILE__ << " (" << __LINE__ << ")" << std::endl;
    }
    return errorCode;
}

Util &Util::getInstance()
{
    static Util instance; // Guaranteed to be destroyed.
                          // Instantiated on first use.
    return instance;
}