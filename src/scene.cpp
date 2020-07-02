#include "scene.h"
#include "core.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"
#include <GL/gl3w.h>
#include <glm/glm.hpp>

void Scene::load(const char *directory, const char *filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::string objPath = std::string(directory) + filename;

    glm::vec3 min(INFINITY);
    glm::vec3 max(-INFINITY);

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

    // Append `default` material
    materials.emplace_back();

    u32 numMeshes = materials.size();
    std::vector<std::vector<Vertex>> meshVertices(numMeshes);
    m_meshes.resize(numMeshes);
    m_materials.resize(numMeshes);

    for (u32 i = 0; i < numMeshes; ++i) {
        m_meshes[i].materialId = i;
        m_materials[i].diffuse = glm::vec4(
                materials[i].diffuse[0],
                materials[i].diffuse[1],
                materials[i].diffuse[2],
                0
        );
        m_materials[i].emissive = glm::vec4(
                materials[i].emission[0],
                materials[i].emission[1],
                materials[i].emission[2],
                0
        );
    }

    for (auto &shape : shapes) {
        // iterate over faces
        for (u32 f = 0; f < shape.mesh.indices.size() / 3; ++f) {
            u32 materialId = shape.mesh.material_ids[f];

            // fix invalid material ids
            if (materialId < 0 || materialId >= materials.size()) {
                materialId = materials.size() - 1;
            }

            std::vector<Vertex> &vertices = meshVertices[materialId];

            // add vertices
            for (u32 v = 0; v < 3; ++v) {
                tinyobj::index_t idx = shape.mesh.indices[3 * f + v];

                vertices.emplace_back();

                // set position
                vertices.back().position = glm::vec4(
                        attrib.vertices[3 * idx.vertex_index + 0],
                        attrib.vertices[3 * idx.vertex_index + 1],
                        attrib.vertices[3 * idx.vertex_index + 2],
                        0
                );

                min = glm::min(min, glm::vec3(vertices.back().position));
                max = glm::max(max, glm::vec3(vertices.back().position));

                // set normal
                if (hasNormals) {
                    vertices.back().normal = glm::vec4(
                            attrib.normals[3 * idx.normal_index + 0],
                            attrib.normals[3 * idx.normal_index + 1],
                            attrib.normals[3 * idx.normal_index + 2],
                            0
                    );
                }
            }

            // calculate normals if missing
            if (!hasNormals) {
                Vertex &v1 = vertices[vertices.size() - 3];
                Vertex &v2 = vertices[vertices.size() - 2];
                Vertex &v3 = vertices[vertices.size() - 1];

                glm::vec3 u = v2.position - v1.position;
                glm::vec3 v = v3.position - v1.position;

                glm::vec4 normal = glm::vec4(glm::cross(u, v), 0);

                v1.normal = normal;
                v2.normal = normal;
                v3.normal = normal;
            }
        }
    }

    for (u32 i = 0; i < numMeshes; ++i) {
        m_meshes[i].vertexOffset = m_vertices.size();
        m_meshes[i].numVertices = meshVertices[i].size();
        m_vertices.insert(m_vertices.end(), meshVertices[i].begin(), meshVertices[i].end());
    }

    core->info("min: " + std::to_string(min.x) + ", " + std::to_string(min.y) + ", " + std::to_string(min.z));
    core->info("max: " + std::to_string(max.x) + ", " + std::to_string(max.y) + ", " + std::to_string(max.z));

    // Generate vertex buffer
    glGenBuffers(1, &m_vertexBuffer);
    if (m_vertexBuffer == 0) {
        core->fatal("Failed to generate vertex buffer");
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_vertexBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(), m_vertices.data(),
                 GL_STATIC_READ);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Generate material buffer
    glGenBuffers(1, &m_materialBuffer);
    if (m_materialBuffer == 0) {
        core->fatal("Failed to generate material buffer");
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_materialBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(m_materials[0]) * m_materials.size(), m_materials.data(),
                 GL_STATIC_READ);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Generate mesh buffer
    glGenBuffers(1, &m_meshBuffer);
    if (m_meshBuffer == 0) {
        core->fatal("Failed to generate mesh buffer");
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_meshBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(m_meshes[0]) * m_meshes.size(), m_meshes.data(), GL_STATIC_READ);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    core->info("Loaded obj with " + std::to_string(m_vertices.size()) + " vertices");
}

void Scene::destroy() {
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_materialBuffer);
    glDeleteBuffers(1, &m_meshBuffer);
}

void Scene::bind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_materialBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_meshBuffer);
}
