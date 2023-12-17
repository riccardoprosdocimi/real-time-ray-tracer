/** @file Error.hpp
 *  @brief Handles OpenGL errors.
 *
 *  Helper header file for error handling in OpenGL.
 */
#ifndef ERROR_HPP
#define ERROR_HPP

#include <glad/glad.h>
#include <iostream>

// This assert inserts a breakpoint in your code.
// That way we can get the line number and file.
// #define ASSERT(x) if(!(x)) __debugbreak(); (__debugbreak() is only available on MSVC)
#define GLCall(x) GLClearErrorStates(); x; GLCheckError(#x,__LINE__);

// New error handling routine
static void GLClearErrorStates(){
    while(glGetError() != GL_NO_ERROR) {} // return all the errors that occur
}

// Returns false if an error occurs
static bool GLCheckError(const char* function, int line) {
    while(GLenum error = glGetError()) {
        // __LINE__ is a special preprocessor macro that can tell us what line any errors occurred on
        std::cout << "[OpenGL Error]" << error << "|" << function << " Line: " << line << "\n";
        return false;
    }
    return true;
}

static_assert(sizeof(GLint) == sizeof(int), "GLint and int are not the same size on this architecture");

static_assert(sizeof(GLuint) == sizeof(unsigned int), "GLuint and unsigned int are not the same size on this architecture");

static_assert(sizeof(GLfloat) == sizeof(float), "GLFloat and float are not the same size on this architecture");

#endif
