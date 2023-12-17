/** @file FrameBuffer.hpp
 *  @brief Creates an offscreen texture to render to.
 *
 *  The 'create' function needs to be called before using the framebuffer.
 *
 *  @bug No known bugs.
 */
#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // this works for Mac
    #include <SDL.h>
#endif

#include <glad/glad.h>
#include <memory>
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

// Each Framebuffer can have a custom shader, so we are forward declaring the class
class Shader;

class FrameBuffer {
public:
    // Constructor
    FrameBuffer();
    // Destructor
    ~FrameBuffer();
    // Creates the framebuffer
    void create(int width, int height);
    // Selects our framebuffer
    void bind() const;
    // Updates our framebuffer once per frame for any changes that may have occurred
    void update(const glm::mat4& projectionMatrix, Camera* camera, int screenWidth, int screenHeight, float time) const;
    // Done with our framebuffer
    static void unbind();
    // Draws a quad to the screen
    void drawFBO() const;
    std::shared_ptr<Shader> m_shader;

private:
    // Creates a quad that will be overlaid on top of the screen
    void setupScreenQuad(float x, float y, float w, float h);
    // Framebuffer id
    GLuint m_fboID;
    // Store our screen buffer
    GLuint m_quadVAO;
    GLuint m_quadVBO;
    // Our framebuffer also needs a texture
    GLuint m_colorBufferID;
    glm::mat4 m_worldTransform;
};

#endif
