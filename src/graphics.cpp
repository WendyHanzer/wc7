#include "engine.hpp"
#include "graphics.hpp"
#include "vertex.hpp"
#include "camera.hpp"
#include "water.hpp"
#include "light.hpp"

#include "gl.hpp"

#include <FreeImagePlus.h>

#include <fstream>
#include <iostream>
#include <utility>

Graphics::Graphics(Engine *eng)
    : engine(eng)
{
    camera = new Camera(engine);
}

Graphics::~Graphics()
{
    if(water)
        delete water;

    if(camera)
        delete camera;
}

void Graphics::init()
{
    #ifdef __APPLE__
    int windowWidth = 1024, windowHeight = 768;
    #else
    int windowWidth = 1600, windowHeight = 900;
    #endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("CS791a", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if(!window) {
        std::cerr << "Unable to create SDL Window" << std::endl;
        exit(1);
    }

    gl_context = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(1);

    initGL();
}

void Graphics::initGL()
{
    #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum status = glewInit();
    if(status != GLEW_OK) {
        std::cerr << "Unable to initialize glew" << std::endl;
        exit(1);
    }
    #endif

    if(engine->getOptions().verbose)
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0.0, 0.2, 0.2, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glEnable(GL_TEXTURE_2D);

    if(engine->getOptions().wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe

    updateView();
    windowResized();


    std::vector<GLuint> shader_data(2);

    shader_data[0] = loadShader("../shaders/water.vs", GL_VERTEX_SHADER);
    shader_data[1] = loadShader("../shaders/water.fs", GL_FRAGMENT_SHADER);
    createShaderProgram("water", shader_data);



    water = new Water(engine, 200, 200);
}

void Graphics::tick(float dt)
{
    water->tick(dt);
}

void Graphics::render()
{
    updateCamera();
    updateView();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    water->render();

    SDL_GL_SwapWindow(window);
}

void Graphics::stop()
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}

void Graphics::updateView()
{
    view = camera->getView();
}

void Graphics::updateCamera()
{
    camera->update();
}

void Graphics::windowResized()
{
    int width, height;
    getWindowSize(width, height);
    glViewport(0, 0, width, height);
    projection = glm::perspective(45.0f, float(width) / float(height),
                                        0.01f, 5000.0f);
}

void Graphics::getWindowSize(int &w, int &h) const
{
    SDL_GetWindowSize(window, &w, &h);
}

GLuint Graphics::getShaderProgram(const std::string& name) const
{
    return programs.at(name);
}

const std::vector<GLuint>& Graphics::getShaderData(const std::string& name) const
{
    return shaders.at(name);
}

GLuint Graphics::loadShader(std::string shaderFile, GLenum shaderType)
{
    std::ifstream fin(shaderFile);
    if(!fin) {
        std::cerr << "Unable to open shader file: " << shaderFile << std::endl;
        engine->stop();
    }

    std::string file_contents;
    fin.seekg(0, std::ios::end);
    auto length = fin.tellg();
    fin.seekg(0, std::ios::beg);

    file_contents.reserve(length);
    auto fileIterator = std::istreambuf_iterator<char>(fin);
    auto fileIteratorEnd = std::istreambuf_iterator<char>();
    file_contents.assign(fileIterator, fileIteratorEnd);

    fin.close();

    const char *shaderStr = file_contents.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderStr, NULL);
    glCompileShader(shader);

    char buffer[512];
    glGetShaderInfoLog(shader, 512, NULL, buffer);
    GLint shader_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status) {
        std::string shaderTypeStr = "shader";
        std::cerr << "Failed to compile " << shaderTypeStr << " loaded from " << shaderFile << std::endl;
        std::cerr << "Compile Error: " << buffer << std::endl;
        //engine->stop();
        exit(1);
    }

    return shader;
}

GLuint Graphics::createShaderProgram(std::string name, std::vector<GLuint> shader_data)
{
    GLuint program = glCreateProgram();
    for(auto shader : shader_data) {
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

    GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status) {
        std::cerr << "Unable to create shader program!" << std::endl;
        //engine->stop();
        exit(1);
    }

    programs[name] = program;
    shaders[name] = shader_data;

    if(engine->getOptions().verbose)
        std::cout << "Created GL Program: " << name << ' ' << program << std::endl;

    return program;
}

GLuint Graphics::createTextureFromFile(std::string fileName, GLenum target)
{
    fipImage image;

    if(image.load(fileName.c_str())) {
        if(image.getImageType() == FIT_UNKNOWN) {
            std::cerr << "Unknow image type for image: " << fileName << std::endl;
            engine->stop(1);
        }

        image.convertTo32Bits();

        GLuint texId;

        glGenTextures(1, &texId);

        if(engine->getOptions().verbose) {
            std::cout << "Texture ID: " << texId << std::endl;
        }

        glBindTexture(target, texId);
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if(target == GL_TEXTURE_2D) {
            glTexImage2D(target, 0, GL_RGBA, image.getWidth(), image.getHeight(),
                0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) image.accessPixels());
        }
        else {
            glTexImage1D(target, 0, GL_RGBA, image.getWidth(), 0,
                GL_RGBA, GL_UNSIGNED_BYTE, (void*) image.accessPixels());
        }

        return texId;
    }

    else {
        std::cerr << "Unable to open image: " << fileName << std::endl;
        return 0;
    }
}