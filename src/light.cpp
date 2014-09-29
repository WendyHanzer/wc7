#include "light.hpp"
#include "engine.hpp"

AmbientLight::AmbientLight(Engine *eng, const glm::vec3& c, float i)
    : engine(eng), color(c), intensity(i)
{
}

DirectionalLight::DirectionalLight(Engine *eng, const glm::vec3 & c, const glm::vec3& dir, float i, float diff)
    : AmbientLight(eng, c, i), direction(dir), diffIntensity(diff)
{
}

PointLight::PointLight(Engine *eng, const glm::vec3& c, const glm::vec3 &p, float i, float diff)
    : AmbientLight(eng, c, i), pos(p), diffIntensity(diff)
{
}