#ifndef MAPLE_SHADER_H
#define MAPLE_SHADER_H

#include "types.h"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

/// OpenGL shader
class Shader {
public:
    /// Load shaders from files
    void init(const char *vertex_path, const char *fragment_path);

    /// Cleanup shaders
    void destroy();

    /// Bind shader for usage
    void bind();

    /// Set vec3 shader uniform
    void setVec3(const std::string &name, glm::vec3 value);

    /// Set mat4 shader uniform
    void setMat4(const std::string &name, glm::mat4 value);

private:
    u32 compileAndAttach(u32 shader_type, const char *shader_src, const char *debug_shader_path);

    u32 getUniformLocation(const std::string &name);

    u32 m_programId = 0;
    std::unordered_map<std::string, u32> m_uniformLocations;
};

#endif //MAPLE_SHADER_H
