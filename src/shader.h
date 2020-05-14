#ifndef MAPLE_SHADER_H
#define MAPLE_SHADER_H

#include "types.h"

class Shader {
public:
    /// Load shaders from files
    void init(const char *vertex_path, const char *fragment_path);

    /// Cleanup shaders
    void destroy();

    /// Bind shader for usage
    void bind();

private:
    u32 m_programId = 0;

    u32 compileAndAttach(u32 shader_type, const char *shader_src, const char *debug_shader_path);
};

#endif //MAPLE_SHADER_H
