#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <vector>
#include <map>
#include <string>

#include "gl.hpp"

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


class Engine;
class Camera;
class Water;
class AmbientLight;
class DirectionalLight;
class PointLight;

class Graphics {
public:
    Graphics(Engine *eng);
    ~Graphics();

    void init();
    void tick(float dt);
    void render();
    void stop();

    void updateView();
    void updateCamera();
    void windowResized();

    void getWindowSize(int &w, int &h) const;
    GLuint getShaderProgram(const std::string& name) const;
    const std::vector<GLuint>& getShaderData(const std::string& name) const;

    GLuint createTextureFromFile(std::string fileName, GLenum target);

    SDL_Window* getRenderWindow() const {return window;}

    glm::mat4 view, projection;

    Camera *camera;
    Water *water;
private:
    void initGL();
    GLuint loadShader(std::string shaderFile, GLenum shaderType);
    GLuint createShaderProgram(std::string name, std::vector<GLuint> shader_data);

    Engine *engine;
    std::map<std::string, GLuint> programs;
    std::map<std::string, std::vector<GLuint>> shaders;

    SDL_Window *window;
    SDL_GLContext gl_context;

    std::vector<AmbientLight*> ambient_lights;
    std::vector<DirectionalLight*> directional_lights;
    std::vector<PointLight*> point_lights;
};

#endif // GRAPHICS_HPP