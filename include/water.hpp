#ifndef WATER_HPP
#define WATER_HPP

#include "gl.hpp"

#include <glm/glm.hpp>

#include <vector>
#include "clock.hpp"

class Engine;
struct Vertex;

class Water
{
public:
    Water(Engine *eng, float w, float h);
    ~Water();

    void init();
    void initGL();
    void tick(float dt);
    void render();

    static glm::vec3 vertexToVec3(const Vertex& vert);
    void setAmbient(float ambient);
    glm::vec3 lightDir;
    float lightAngle;
private:
    Engine *engine;
    float width, height;
    float density, specIntensity, specPower, dt, ambient;
    Clock clock;

    GLuint program;
    GLuint vbo, vao, vio;

    GLint loc_pos, loc_mvp, loc_view, loc_normal, loc_texture, loc_model, loc_modelView, loc_specPower, 
        loc_specIntensity, loc_camPos, loc_time, loc_ambient, loc_sampler;
    GLint loc_lightDir;

    std::vector<Vertex> geometry;
    std::vector<glm::vec3> faceNormals;
    glm::mat4 model;
    GLuint texture;
    std::vector<unsigned int> indices;
};

#endif // WATER_HPP