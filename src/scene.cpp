#include "scene.h"
#include "core.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"
#include <GL/gl3w.h>

void Scene::load(const char *directory, const char *filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::string objPath = std::string(directory) + filename;

    bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath.c_str(), directory);
    if (!warn.empty()) {
        core->warn(warn);
    }
    if (!err.empty()) {
        core->warn(err);
    }
    if (!result) {
        core->fatal("Failed to load obj \"" + objPath + "\"");
    }

    bool hasNormals = !attrib.normals.empty();

    for (auto &shape : shapes) {
        // iterate over faces
        for (u32 f = 0; f < shape.mesh.indices.size() / 3; ++f) {
            // add vertices
            for (u32 v = 0; v < 3; ++v) {
                tinyobj::index_t idx = shape.mesh.indices[3 * f + v];

                m_vertices.emplace_back();

                // set position
                m_vertices.back().position = glm::vec4(
                        attrib.vertices[3 * idx.vertex_index + 0],
                        attrib.vertices[3 * idx.vertex_index + 1],
                        attrib.vertices[3 * idx.vertex_index + 2],
                        0
                );

                // set normal
                if (hasNormals) {
                    m_vertices.back().normal = glm::vec4(
                            attrib.normals[3 * idx.normal_index + 0],
                            attrib.normals[3 * idx.normal_index + 1],
                            attrib.normals[3 * idx.normal_index + 2],
                            0
                    );
                }
            }

            // calculate normals if missing
            if (!hasNormals) {
                Vertex &v1 = m_vertices[m_vertices.size() - 3];
                Vertex &v2 = m_vertices[m_vertices.size() - 2];
                Vertex &v3 = m_vertices[m_vertices.size() - 1];

                glm::vec3 u = v2.position - v1.position;
                glm::vec3 v = v3.position - v1.position;

                glm::vec4 normal = glm::vec4(glm::normalize(glm::cross(u, v)), 0);

                v1.normal = normal;
                v2.normal = normal;
                v3.normal = normal;
            }
        }
    }

    glGenBuffers(1, &m_ssbo);
    if (m_ssbo == 0) {
        core->fatal("Failed to generate ssbo");
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(), m_vertices.data(), GL_STATIC_READ);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    core->info("Loaded obj with " + std::to_string(m_vertices.size()) + " vertices");
}

void Scene::destroy() {
    glDeleteBuffers(1, &m_ssbo);
}

void Scene::bindSsbo(u32 binding) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_ssbo);
}
