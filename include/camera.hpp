#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Engine;

class Camera
{
public:
    Camera(Engine *eng);

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void rotate(float x, float y);
    void update();

    void applyBoost(float newBoost);

    glm::mat4 getView() const;
    glm::vec3 getPos() const { return pos; }

private:
    Engine *engine;
    glm::vec3 pos, orientation ,up;
    float rotateX, rotateY, speed, sensitivity, boost;
};

#endif // CAMERA_HPP
