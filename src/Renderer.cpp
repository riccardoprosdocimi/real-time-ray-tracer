#include "Renderer.hpp"

// Constructor
// Sets the height and width of our renderer
Renderer::Renderer(int w, int h) {
    m_screenWidth = w;
    m_screenHeight = h;
    m_camera = new Camera(); // create one camera within the renderer
    m_frameBuffer = new FrameBuffer(); // create one framebuffer within the renderer
    m_frameBuffer -> create(w, h);
}

// Destructor
Renderer::~Renderer() {
    delete m_camera; // delete camera pointer
    delete m_frameBuffer; // delete framebuffer pointer
}

// Renders the scene
void Renderer::render(float time) {
    // Here we apply the projection matrix which creates perspective.
    // The first argument is 'field of view'
    // Then perspective
    // Then the near and far clipping plane
    // Note I cannot see anything closer than 0.1f units from the screen
    m_projectionMatrix = glm::perspective(glm::radians(45.0f), ((float)m_screenWidth) / ((float)m_screenHeight), 0.1f, 512.0f);
    m_frameBuffer -> update(m_projectionMatrix, m_camera, m_screenWidth, m_screenHeight, time); // update our framebuffer
    m_frameBuffer -> bind(); // select our framebuffer
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // this is the background of the screen
    // You should only see a single quad after rendering this, because we are only drawing one quad
    // This is how we know things are working with our FBO
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_W]) { // press the 'w' key to toggle wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    m_frameBuffer -> unbind(); // finish with our framebuffer
    // Now draw a new scene
    // Clear everything away
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // clear the screen color, and typically I do this to something 'different' than our original as an indication that I am in an FBO.
    glClear(GL_COLOR_BUFFER_BIT); // we only have 'color' in our buffer that is stored
    m_frameBuffer->m_shader->bind(); // use our new 'simple screen shader'
    m_frameBuffer->drawFBO(); // overlay our 'quad' over the screen
    m_frameBuffer->m_shader->unbind(); // unselect our shader and continue
}
