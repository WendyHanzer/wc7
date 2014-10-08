#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "gl.hpp"

struct Vertex {
    GLfloat pos[3];
    GLfloat normal[3];
    GLfloat texture[2];
};

#endif // VERTEX_HPP