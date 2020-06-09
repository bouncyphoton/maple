#ifndef MAPLE_SCENE_H
#define MAPLE_SCENE_H

#include "types.h"
#include "vertex.h"
#include <vector>

class Scene {
public:
    /// Load an .obj file as the scene
    void load(const char *directory, const char *filename);

    /// Clean up scene
    void destroy();

    /// Bind scene for path-tracing
    void bindSsbo(u32 binding);

private:
    u32 m_ssbo;
    std::vector<Vertex> m_vertices;
};

#endif //MAPLE_SCENE_H
