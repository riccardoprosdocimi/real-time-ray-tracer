/** @file SDLGraphicsProgram.hpp
 *  @brief SDL class used to set up input and OpenGL.
 *  
 *  This class is used for the initialization of SDL.
 *  It sets up a full graphics program using SDL.
 *
 *  @bug No known bugs.
 */
#ifndef SDL_GRAPHICS_PROGRAM_HPP
#define SDL_GRAPHICS_PROGRAM_HPP
// ==================== Libraries ==================
// Depending on the operating system we use, the paths to SDL are actually different.
// The #define statement should be passed in when compiling using the -D argument.
// This gives an example of how a programmer may support multiple platforms with different dependencies.
#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // this works for Mac
    #include <SDL.h>
#endif

// C++ libraries
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <chrono>
// Project libraries
#include "Renderer.hpp"

class SDLGraphicsProgram {
public:
    // Constructor
    SDLGraphicsProgram(int w, int h);
    // Destructor
    ~SDLGraphicsProgram();
    // Loops forever
    void loop();
    // Gets pointer to window
    inline SDL_Window* getSDLWindow() {
        return m_window;
    }
    // Queries OpenGL information
    static void getOpenGLVersionInfo();

private:
    // The window we'll be rendering to
    SDL_Window* m_window ;
    // OpenGL context
    SDL_GLContext m_openGLContext;
    // Window's width and height
    int m_width;
    int m_height;
};

#endif
