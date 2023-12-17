/** @file Camera.hpp
 *  @brief Sets up an OpenGL camera.
 *
 *  The camera is what sets up our 'view' matrix.
 *
 *  @bug No known bugs.
 */
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Camera {
public:
	// Constructor
    Camera();
    // Destructor
    ~Camera();
    // Returns a 'view' matrix with our camera transformation applied
    glm::mat4 getWorldToViewMatrix() const;
    // Moves the camera around
    void mouseLook(int mouseX, int mouseY);
    // Moves the camera forward
    void moveForward(float speed);
    // Moves the camera backward
    void moveBackward(float speed);
    // Moves the camera leftward
    void moveLeft(float speed);
    // Moves the camera rightward
    void moveRight(float speed);
    // Moves the camera upward
    void moveUp(float speed);
    // Moves the camera downward
    void moveDown(float speed);
    // Sets the position for the camera
    void setCameraEyePosition(float x, float y, float z);
    // Returns the camera X position where the eye is
    inline float getEyeXPosition() const {
        return m_eyePosition.x;
    };
    // Returns the camera Y position where the eye is
    inline float getEyeYPosition() const {
        return m_eyePosition.y;
    };
    // Returns the camera Z position where the eye is
    inline float getEyeZPosition() const {
        return m_eyePosition.z;
    };
	// Returns the X 'view' direction
    inline float getViewXDirection() const {
        return m_viewDirection.x;
    };
    // Returns the Y 'view' direction
    inline float getViewYDirection() const {
        return m_viewDirection.y;
    };
    // Returns the Z 'view' direction
    inline float getViewZDirection() const {
        return m_viewDirection.z;
    };

private:
    // Tracks the old mouse position
    glm::vec2 m_oldMousePosition;
    // Tracks where our camera is positioned
    glm::vec3 m_eyePosition;
    // Tracks what direction the camera is looking
    glm::vec3 m_viewDirection;
    // Tracks which direction is 'up' in our world
    glm::vec3 m_upVector;
};

#endif
