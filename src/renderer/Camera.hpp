#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <chrono>

// Does not compile on Windows without these.
#undef near
#undef far
// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Defines several possible options for camera movement. Used as
    // abstraction to stay away from window-system specific input methods
    enum class Movement
    {
        Forward,
        Backward,
        Left,
        Right
    };

    // constructor with vectors
    Camera(const glm::vec3 &position = glm::vec3(0.f),
           const glm::vec3 &up = glm::vec3(0.f, 1.f, 0.f),
           float yaw = -90.f,
           float pitch = 0.f,
           float speed = 15.5f,
           float sensitivity = 0.1f,
           float zoom = 45.f);

    // processes input received from any keyboard-like input system.
    // Accepts input parameter in the form of camera defined ENUM
    // (to abstract it from windowing systems)
    void processKeyboard(Movement direction,
                         std::chrono::nanoseconds deltaTimem);

    // processes input received from a mouse input system. Expects
    // the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset,
                              bool constrainPitch = true);

    // processes input received from a mouse scroll-wheel event.
    // Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset);

    // Create perspective matrix.
    inline glm::mat4 createPerspective(float aspect,
                                       float near = 0.1f,
                                       float far = 1000.f) const
    {
        return glm::perspective(mZoom, aspect, near, far);
    }

    // Get camera position.
    inline glm::vec3 getPosition() const
    {
        return mPosition;
    }

    // returns the view matrix calculated using Euler Angles and
    // the LookAt Matrix
    inline glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(mPosition, mPosition + mFront, mUp);
    }

private:
    // camera Attributes
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;
    // euler Angles
    float mYaw;
    float mPitch;
    // camera options
    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;

    // calculates the front vector from the Camera's (updated) Euler
    // Angles
    void updateCameraVectors();
};
#endif
