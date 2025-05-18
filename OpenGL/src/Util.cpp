#include "util.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        SET_CLI_RED();
        std::cerr << "[OpenGL Error] (" << error << "): " << function << ' ' << file << ':' << line << std::endl;
        RESET_CLI();
        return false;
    }
    return true;
}
