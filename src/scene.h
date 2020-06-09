#ifndef MAPLE_SCENE_H
#define MAPLE_SCENE_H

#include "types.h"
#include "vertex.h"
#include "material.h"
#include "mesh.h"
#include <vector>

class Scene {
public:
    /// Load an .obj file as the scene
    void load(const char *directory, const char *filename);

    /// Clean up scene
    void destroy();

    /// Bind scene for path-tracing
    void bind();

private:
    u32 m_vertexBuffer;
    u32 m_materialBuffer;
    u32 m_meshBuffer;
    std::vector<Vertex> m_vertices;
    std::vector<Material> m_materials;
    std::vector<Mesh> m_meshes;
};

#endif //MAPLE_SCENE_H
