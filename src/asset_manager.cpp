#include "asset_manager.h"
#include "core.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"
#include <vector>

void AssetManager::loadModelFromObj(const char *directory, const char *filename) {
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

    Model &model = m_pathModelMap[objPath];

    // Append `default` material
    materials.emplace_back();

    u32 numMeshes = materials.size();
    std::vector<std::vector<Vertex>> meshVertices(numMeshes);
    model.meshes.resize(numMeshes);

    for (auto &shape : shapes) {
        // iterate over faces
        for (u32 f = 0; f < shape.mesh.indices.size() / 3; ++f) {
            u32 materialId = shape.mesh.material_ids[f];

            // fix invalid material ids
            if (materialId < 0 || materialId >= materials.size()) {
                materialId = materials.size() - 1;
            }

            model.meshes[materialId].diffuseColor = glm::vec3(
                    materials[materialId].diffuse[0],
                    materials[materialId].diffuse[1],
                    materials[materialId].diffuse[2]
            );

            model.meshes[materialId].emissionColor = glm::vec3(
                    materials[materialId].emission[0],
                    materials[materialId].emission[1],
                    materials[materialId].emission[2]
            );

            std::vector<Vertex> &vertices = meshVertices[materialId];

            // add vertices
            for (u32 v = 0; v < 3; ++v) {
                tinyobj::index_t idx = shape.mesh.indices[3 * f + v];

                vertices.emplace_back();

                vertices.back().position = glm::vec3(
                        attrib.vertices[3 * idx.vertex_index + 0],
                        attrib.vertices[3 * idx.vertex_index + 1],
                        attrib.vertices[3 * idx.vertex_index + 2]
                );
            }
        }
    }

    // init meshes and remove meshes with 0 vertices
    for (u32 m = 0; m < model.meshes.size();) {
        if (meshVertices[m].empty()) {
            model.meshes.erase(model.meshes.begin() + m);
            meshVertices.erase(meshVertices.begin() + m);
        } else {
            model.meshes[m].init(meshVertices[m]);
            ++m;
        }
    }
}

void AssetManager::destroy() {
    for (auto &pathModelPair : m_pathModelMap) {
        for (auto &mesh : pathModelPair.second.meshes) {
            mesh.destroy();
        }
    }
}

Model &AssetManager::getModel(const std::string &path_to_model) {
    return m_pathModelMap[path_to_model];
}
