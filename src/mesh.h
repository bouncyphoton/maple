#ifndef MAPLE_MESH_H
#define MAPLE_MESH_H

#include "types.h"
#include "vertex.h"
#include "material.h"
#include <vector>

class Mesh {
public:
    void init(const std::vector<Vertex> &vertices);

    void destroy();

    void draw();

    Material material;

private:
    u32 m_vao, m_vbo;
    std::vector<Vertex> m_vertices;
};

#endif //MAPLE_MESH_H
