#include "Camera.hpp"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, float yaw,
               float pitch, float speed, float sensitivity, float zoom)
    : mPosition(position),mFront(glm::vec3(0.f, 0.f, -1.f)),mUp(0.f),
      mRight(0.f),mWorldUp(up),mYaw(0.f),mPitch(0.f),
      mMovementSpeed(speed),mMouseSensitivity(sensitivity),mZoom(zoom)
{
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
    // also re-calculate the Right and Up vector
    // normalize the vectors, because their length gets closer to 0
    // the more you look up or down which results in slower movement.
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp = glm::normalize(glm::cross(mRight, mFront));
}

void Camera::processMouseScroll(float yoffset)
{
    mZoom -= yoffset;
    if(mZoom < 1.0f)
        mZoom = 1.0f;
    else if(mZoom > 45.0f)
        mZoom = 45.0f; 
}

void Camera::processMouseMovement(float xoffset, float yoffset,
                                  bool constrainPitch)
{
    xoffset *= mMouseSensitivity;
    yoffset *= mMouseSensitivity;

    mYaw   += xoffset;
    mPitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't
    // get flipped
    if(constrainPitch)
    {
        if(mPitch > 89.0f)
            mPitch = 89.0f;
        else if(mPitch < -89.0f)
            mPitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler
    // angles
    updateCameraVectors();
}

void Camera::processKeyboard(Camera::Movement direction, float deltaTime)
{
    float velocity = mMovementSpeed * deltaTime;
    switch(direction)
    {
    case Camera::Movement::Forward:
        mPosition += mFront * velocity;
        break;
    case Camera::Movement::Backward:
        mPosition -= mFront * velocity;
        break;
    case Camera::Movement::Left:
        mPosition -= mRight * velocity;
        break;
    case Camera::Movement::Right:
        mPosition += mRight * velocity;
        break;
    }
}
