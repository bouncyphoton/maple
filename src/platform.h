#ifndef MAPLE_PLATFORM_H
#define MAPLE_PLATFORM_H

#include "SDL.h"

/// Platform abstraction for application
class Platform {
public:
    /// Initialize platform code
    void init();

    /// Cleanup
    void destroy();

    /// Update inputs, etc.
    void update();

    /// Swap buffers for rendering
    void swapBuffers();

private:
    SDL_Window *m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
};

#endif //MAPLE_PLATFORM_H
