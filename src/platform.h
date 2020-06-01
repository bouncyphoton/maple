#ifndef MAPLE_PLATFORM_H
#define MAPLE_PLATFORM_H

#include "types.h"
#include "SDL.h"
#include <unordered_map>
#include <chrono>

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

    /// Query if key was newly pressed since last update
    bool isKeyPressed(u32 key);

    /// Query if key is pressed/down
    bool isKeyDown(u32 key);

    /// Get time in milliseconds since last update
    f32 getFrameTime();

private:
    enum KeyStateEnum {
        KEY_STATE_UP,
        KEY_STATE_DOWN,
        KEY_STATE_PRESSED,
    };

    SDL_Window *m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    std::unordered_map<u32, KeyStateEnum> m_keyStates;

    std::chrono::system_clock::time_point m_lastUpdate;
    f32 m_frameTime;
};

#endif //MAPLE_PLATFORM_H
