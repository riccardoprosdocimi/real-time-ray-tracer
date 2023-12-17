#include "FrameBuffer.hpp"

// Constructor
FrameBuffer::FrameBuffer() {
    m_shader = std::make_shared<Shader>(); // initialize the shader
    // Set up the shaders for the Frame Buffer Object
    std::string fboVertexShader = m_shader->loadShader("./shaders/vert.glsl");
    std::string fboFragmentShader = m_shader->loadShader("./shaders/frag.glsl");
    m_shader->createShader(fboVertexShader, fboFragmentShader); // create our shaders
    // Set up the quad to draw to
    // x and y of 0.0 put the quad in the top left corner
    // w and h of 1.0 stretch quad across entire screen
    setupScreenQuad(0.0f, 0.0f, 1.0f, 1.0f); // set up the screen quad
}

// Destructor
FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &m_fboID);
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteBuffers(1, &m_quadVBO);
}

// Creates the framebuffer
// We create this in a second step because we need width and height information
void FrameBuffer::create(int width, int height) {
    glGenFramebuffers(1, &m_fboID); // generate a framebuffer
    bind(); // select the buffer we have just generated
    // Create a color attachment texture
    glGenTextures(1, &m_colorBufferID);
    glBindTexture(GL_TEXTURE_2D, m_colorBufferID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBufferID, 0);
    unbind(); // deselect our buffer
}

// Selects our framebuffer
void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
}

// Updates our framebuffer once per frame for any changes that may have occurred
void FrameBuffer::update(const glm::mat4 &projectionMatrix, Camera* camera, int screenWidth, int screenHeight, float time) const {
    glm::vec2 screenDimensions(screenWidth, screenHeight);
    m_shader -> bind(); // select our framebuffer
    // Set the uniforms in our current shader
    m_shader -> setUniform1i("u_diffuseMap", 0); // note that we set the value to 0, because we have bound our texture to slot 0
    m_shader -> setUniform1f("u_time", time);
    m_shader -> setUniform2fv("u_resolution", &screenDimensions[0]);
    m_shader -> setUniform1f("u_camSpin", camera -> getEyeYPosition());
}

// Done with our framebuffer
void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Draws a quad to the screen
// This is the actual rendering of our FBO to the screen
// Typically, this would be called after 'update'
void FrameBuffer::drawFBO() const {
    glBindVertexArray(m_quadVAO);
    glBindTexture(GL_TEXTURE_2D, m_colorBufferID); // use the color attachment texture as the texture of the quad plain
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// Creates a quad that will be overlaid on top of the screen
// x and y specify the position
// w and h specify the width and the height
void FrameBuffer::setupScreenQuad(float x, float y, float w, float h) {
    float quad[] = {
        // First triangle
        x-w, y+h,  0.0f, 1.0f,
        x-w, y-h,  0.0f, 0.0f,
        x+w, y-h,  1.0f, 0.0f,
        // Second triangle
        x+-w, y+h, 0.0f, 1.0f,
        x+ w, y-h, 1.0f, 0.0f,
        x+ w, y+h, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &m_quadVAO);
    glBindVertexArray(m_quadVAO);

    glGenBuffers(1, &m_quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
