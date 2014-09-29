#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "gl.hpp"

#include <glm/glm.hpp>

class Engine;

class AmbientLight
{
public:
    AmbientLight(Engine *eng, const glm::vec3& c, float i);

    Engine *engine;
    glm::vec3 color;
    float intensity;
};

class DirectionalLight : public AmbientLight
{
public:
    DirectionalLight(Engine *eng, const glm::vec3 & c, const glm::vec3& dir, float i, float diff);

    glm::vec3 direction;
    float diffIntensity;
};

class PointLight : public AmbientLight
{
public:
    PointLight(Engine *eng, const glm::vec3& c, const glm::vec3 &p, float i, float diff);

    struct {
        float constant, linear, exp;
    } Attenuation;

    glm::vec3 pos;
    float diffIntensity;
};

#endif // LIGHT_HPP