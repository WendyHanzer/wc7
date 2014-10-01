#include "water.hpp"
#include "engine.hpp"
#include "vertex.hpp"
#include "graphics.hpp"
#include "light.hpp"

#include <random>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

Water::Water(Engine *eng, float w, float h)
    : engine(eng), width(w), height(h)
{
    lightAngle = 0.0f;
    lightDir = glm::vec3(0,-1,0);
    density = engine->getOptions().density;
    init();
}

Water::~Water()
{
}

void Water::init()
{
    Vertex vert = {{0.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f}};
    vert.pos[1] = 0.0f;

    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 2.0f);

    /*
    bool firstx = true;
    bool firstz = true;
    for(float x = 0.0f; x < width; x += density) {
        for(float z = 0.0f; z < height; z += density) {
            if(firstz || firstx) {
                vert.pos[0] = x;
                vert.pos[1] = 0;//dist(gen);
                vert.pos[2] = z;
                geometry.push_back(vert);


                //float y_2_5 = dist(gen);

                vert.pos[0] = x;
                vert.pos[1] = 0;//dist(gen);
                vert.pos[2] = z+density;
                geometry.push_back(vert);

                firstz = false;
            }

            //float y_3_4 = 0;//dist(gen);

            vert.pos[0] = x+density;
            vert.pos[1] = 0;//dist(gen);
            vert.pos[2] = z;
            geometry.push_back(vert);

            /*
            vert.pos[0] = x+density;
            vert.pos[1] = dist(gen);
            vert.pos[2] = z;
            geometry.push_back(vert);

            vert.pos[0] = x;
            vert.pos[1] = dist(gen);
            vert.pos[2] = z+density;
            geometry.push_back(vert);
            

            vert.pos[0] = x+density;
            vert.pos[1] = dist(gen);
            vert.pos[2] = z+density;
            geometry.push_back(vert);
        }
        

        firstx = false;
        firstz = true;
    }*/

    for(float x = 0.0f; x < width; x += density) {
        for(float z = 0.0f; z < height; z += density) {
            vert.pos[0] = x;
            vert.pos[1] = dist(gen);
            vert.pos[2] = z;

            geometry.push_back(vert);
        }
    }


    unsigned int index = 0, scaledWidth = width/density, scaledHeight = height/density;    
    for(int x = 0; x < scaledWidth - 1; x++) {
        for(int z = 0; z < scaledHeight - 1; z++) {
            indices.push_back(z * scaledHeight + x);
            indices.push_back((z*scaledHeight) + x + 1);
            indices.push_back((z+1) * scaledHeight + x);

            indices.push_back((z * scaledHeight) + x + 1);
            indices.push_back((z+1) * scaledHeight + x + 1);
            indices.push_back((z + 1) * scaledHeight + x);
        }
        /*
        indices.push_back(index);
        indices.push_back(index+1);
        indices.push_back(index+2);
        indices.push_back(index+2);
        indices.push_back(index+1);
        indices.push_back(index+3);

        index += 2;
        */
    }

    model = glm::translate(model, glm::vec3(-width/2.0f, 0.0f, -height/2.0f));

    glm::vec3 v1, v2, v3, arrow1, arrow2, normal;

    for(int i = 0; i < indices.size(); i += 3) {
        v1 = vertexToVec3(geometry[indices[i]]);
        v2 = vertexToVec3(geometry[indices[i+1]]);
        v3 = vertexToVec3(geometry[indices[i+2]]);

        arrow1 = v2 - v1;
        arrow2 = v3 - v1;

        normal = glm::normalize(glm::cross(arrow1, arrow2));

        for(int j = 0; j < 3; j++) {
            geometry[indices[i]].normal[j] = normal[j];
            geometry[indices[i+1]].normal[j] = normal[j];
            geometry[indices[i+2]].normal[j] = normal[j];
        }

        //std::cout << "norm: " << glm::to_string(normal) << std::endl;
    }

    initGL();
}

void Water::initGL()
{
    program = engine->graphics->getShaderProgram("water");

    loc_mvp = glGetUniformLocation(program, "mvp");
    loc_view = glGetUniformLocation(program, "view");
    loc_pos = glGetAttribLocation(program, "vs_position");
    loc_normal = glGetAttribLocation(program, "vs_normal");
    loc_model = glGetUniformLocation(program, "model");
    loc_modelView = glGetUniformLocation(program, "modelView");
    loc_lightDir = glGetUniformLocation(program, "light_dir");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    Vertex *geo = geometry.data();

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * geometry.size(), geo, GL_STATIC_DRAW);

    glGenBuffers(1, &vio);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void Water::tick(float dt)
{
    float y = -cos(lightAngle);
    float x = -sin(lightAngle);

    lightDir[1] = y;
    lightDir[0] = x;

    //std::cout << "x: " << x << " y: " << y << std::endl;
}

void Water::render()
{
    glm::mat4 mvp = engine->graphics->projection * engine->graphics->view * model;
    glm::mat4 modelView = engine->graphics->view * model;

    glUseProgram(program);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(engine->graphics->view));
    glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(loc_modelView, 1, GL_FALSE, glm::value_ptr(modelView));
    glUniform3fv(loc_lightDir, 1, glm::value_ptr(lightDir));

    glEnableVertexAttribArray(loc_pos);
    glEnableVertexAttribArray(loc_normal);

    glVertexAttribPointer( loc_pos,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                           (void*)offsetof(Vertex,pos));

    glVertexAttribPointer( loc_normal,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                           (void*)offsetof(Vertex,normal));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    // draw object
    //glDrawArrays(GL_TRIANGLES, 0, geometry.size());//mode, starting index, count
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    // disable attribute pointers
    glDisableVertexAttribArray(loc_pos);
    glDisableVertexAttribArray(loc_normal);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(0);
}

glm::vec3 Water::vertexToVec3(const Vertex& vert)
{
    return glm::vec3(vert.pos[0], vert.pos[1], vert.pos[2]);
}