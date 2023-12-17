/** @file Shader.hpp
 *  @brief Manages the loading, compiling, and linking of vertex and fragment shaders.
 *  
 *  Additionally, it has functions for setting various uniforms.
 *
 *  @bug No known bugs.
 */
#ifndef SHADER_HPP
#define SHADER_HPP

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // this works for Mac
    #include <SDL.h>
#endif

#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <glad/glad.h>

class Shader {
public:
    // Constructor
    Shader();
    // Destructor
    ~Shader();
    // Selects a shader
    void bind() const;
    // Deselects a shader
    static void unbind() ;
    // Loads a shader
    static std::string loadShader(const std::string &fileName);
    // Creates a shader from a loaded vertex and fragment shaders
    void createShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
    // Returns the shader ID
    inline GLuint getID() const {
        return m_shaderID;
    }
    // Sets the uniforms for a shader
    void setUniformMatrix4fv(const GLchar* name, const GLfloat* value) const;
	void setUniform2fv(const GLchar* name, const GLfloat* value) const;
    void setUniform1i(const GLchar* name, int value) const;
    void setUniform1f(const GLchar* name, float value) const;

private:
    // Compiles loaded shaders
    static unsigned int compileShader(unsigned int type, const std::string& source);
    // Checks if shaders 'linked' successfully
    static bool checkLinkStatus(GLuint programID);
    // Logs an error message
    static void log(const char* system, const char* message);
    // The unique shaderID
    GLuint m_shaderID;
};

#endif
