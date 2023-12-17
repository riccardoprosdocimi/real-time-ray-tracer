/** @file Renderer.hpp
 *  @brief Renderer is responsible for drawing.
 *
 * 	Renderer is responsible for drawing everything. It contains a framebuffer and a camera.
 *
 *  @bug No known bugs.
 */
#ifndef RENDERER_HPP
#define RENDERER_HPP

// The glad library helps set up OpenGL extensions.
// This renderer is designed specifically for OpenGL.
#include <glad/glad.h>
#include "glm/gtc/matrix_transform.hpp"
#include "Error.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "FrameBuffer.hpp"


class Renderer {
public:
    // Constructor
    Renderer(int w, int h);
    // Destructor
    ~Renderer();
    // Renders the scene
    void render(float time);
    // Returns the camera
    inline Camera* getCamera() {
        return m_camera;
    }

private:
    Camera* m_camera;
    // Store the projection matrix for our camera
    glm::mat4 m_projectionMatrix;
    FrameBuffer* m_frameBuffer;
    // Screen dimensions constants
    int m_screenWidth;
    int m_screenHeight;
};

#endif
