#ifndef WATER_HPP
#define WATER_HPP

#include "gl.hpp"

#include <glm/glm.hpp>

#include <vector>

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

    glm::vec3 lightDir;
    float lightAngle;
private:
    Engine *engine;
    float width, height;
    float density;

    GLuint program;
    GLuint vbo, vao, vio;

    GLint loc_pos, loc_mvp, loc_view, loc_normal, loc_model, loc_modelView;
    GLint loc_lightDir;

    std::vector<Vertex> geometry;
    glm::mat4 model;
    std::vector<unsigned int> indices;
};

#endif // WATER_HPP