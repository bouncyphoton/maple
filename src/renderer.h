#ifndef MAPLE_RENDERER_H
#define MAPLE_RENDERER_H

#include "shader.h"

/// Handles all rendering
class Renderer {
public:
    /// Initialize renderer
    void init();

    /// Cleanup renderer
    void destroy();

    /// Render a frame to the default framebuffer
    void render();

    /// Resize renderer
    void resize();

private:
    u32 m_dummyVao = 0;
    u32 m_outTexture = 0;
    u32 m_frameNumber = 0;
    Shader m_fullscreenTextureShader;
    Shader m_meshShader;
    ComputeShader m_pathTraceShader;
};

#endif //MAPLE_RENDERER_H
