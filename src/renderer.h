#ifndef MAPLE_RENDERER_H
#define MAPLE_RENDERER_H

#include "shader.h"

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
    Shader m_skyShader;
};

#endif //MAPLE_RENDERER_H
