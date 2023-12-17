#include "Camera.hpp"

// Constructor
Camera::Camera() {
    m_eyePosition = glm::vec3(0.0f, 0.0f, 0.0f); // position the camera at the origin
    m_viewDirection = glm::vec3(0.0f, 0.0f, -1.0f); // looking down along the z-axis initially (negative because we are looking 'into' the scene)
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f); // upVector always points up along the y-axis
}

// Destructor
Camera::~Camera() = default;

// Moves the camera around
void Camera::mouseLook(int mouseX, int mouseY) {
    glm::vec2 newMousePosition(mouseX, mouseY); // record our new position as a vector
    glm::vec2 speed = 0.01f * (newMousePosition - m_oldMousePosition); // detect how much the mouse has moved since the last time
    m_viewDirection = glm::mat3(glm::rotate(-speed.x, m_upVector)) * m_viewDirection; // set what direction the camera is looking
    m_oldMousePosition = newMousePosition; // update our old position after we have made changes
}

// Moves the camera forward
void Camera::moveForward(float speed) {
    m_eyePosition += m_viewDirection * speed;
}

// Moves the camera backward
void Camera::moveBackward(float speed) {
//    m_eyePosition -= m_viewDirection * speed;
    m_eyePosition.y = 0.0;
}

// Moves the camera leftward
void Camera::moveLeft(float speed) {
//    glm::vec3 rightVector = glm::cross(m_upVector, m_viewDirection);
//    m_eyePosition += speed * rightVector;
    m_eyePosition.y = 1.0;
}

// Moves the camera rightward
void Camera::moveRight(float speed) {
//    glm::vec3 rightVector = glm::cross(m_upVector, m_viewDirection);
//    m_eyePosition -= speed * rightVector;
    m_eyePosition.y = -1.0;
}

// Moves the camera upward
void Camera::moveUp(float speed) {
    m_eyePosition.y += speed;
}

// Moves the camera downward
void Camera::moveDown(float speed) {
    m_eyePosition.y -= speed;
}

// Sets the position for the camera
void Camera::setCameraEyePosition(float x, float y, float z){
    m_eyePosition = glm::vec3(x, y, z);
}

// Returns a 'view' matrix with our camera transformation applied
glm::mat4 Camera::getWorldToViewMatrix() const {
    return glm::lookAt( m_eyePosition,
                        m_eyePosition + m_viewDirection,
                        m_upVector);
}
