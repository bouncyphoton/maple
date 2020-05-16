#ifndef MAPLE_ASSET_MANAGER_H
#define MAPLE_ASSET_MANAGER_H

#include "types.h"
#include "model.h"
#include <unordered_map>
#include <vector>

class AssetManager {
public:
    /// Load a model from a .obj file
    void loadModelFromObj(const char *directory, const char *filename);

    /// Clean up loaded models
    void destroy();

    /// Get model reference by path to model
    Model &getModel(const std::string &path_to_model);

private:
    std::unordered_map<std::string, Model> m_pathModelMap;
};

#endif //MAPLE_ASSET_MANAGER_H
