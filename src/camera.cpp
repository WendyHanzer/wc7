#include "camera.hpp"
#include "engine.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(Engine *eng)
    : engine(eng),
      pos(0.0f, 50.0f, 200.0f),
      orientation(0.0f, -50.0f, -200.0f),
      up(0.0f,1.0f,0.0f)
{
    rotateX = rotateY = 0.0f;
    boost = 1.0f;
    speed = engine->getOptions().camera_speed;
    sensitivity = engine->getOptions().camera_sensitivity;
}

void Camera::moveForward()
{
    pos += (glm::normalize(orientation) * speed * boost);
}

void Camera::moveBackward()
{
    pos -= (glm::normalize(orientation) * speed * boost);
}

void Camera::moveLeft()
{
    pos -= glm::cross((glm::normalize(orientation) * speed * boost), up);
}

void Camera::moveRight()
{
    pos += glm::cross((glm::normalize(orientation) * speed * boost), up);
}

void Camera::moveUp()
{
    pos += (up * speed * boost);
}

void Camera::moveDown()
{
    pos -= (up * speed * boost);
}

void Camera::rotate(float x, float y)
{
    rotateX -= x * sensitivity;
    rotateY += y * sensitivity;
}

void Camera::update()
{
    if(rotateX != 0.0f)
        orientation = glm::rotate(orientation, rotateX, up);

    if(rotateY != 0.0f)
        orientation = glm::rotate(orientation, rotateY, glm::cross(up, orientation));

    rotateX = rotateY = 0.0f;
}

glm::mat4 Camera::getView() const
{
    return glm::lookAt(pos,
                       pos + orientation,
                       up);
}

void Camera::applyBoost(float newBoost)
{
  boost = newBoost;
}
