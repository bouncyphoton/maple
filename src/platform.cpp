#include "platform.h"
#include "core.h"
#include "constants.h"
#include <GL/gl3w.h>

void Platform::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        core->fatal("Failed to init SDL");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, consts::OPENGL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, consts::OPENGL_VERSION_MINOR);

    // sdl window
    m_window = SDL_CreateWindow(consts::PROJECT_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                core->state.frameWidth, core->state.frameHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!m_window) {
        core->fatal("Failed to create window");
    }

    // opengl context
    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        core->fatal("Failed to create OpenGL context");
    }

    // gl3w
    if (gl3wInit()) {
        core->fatal("Failed to initialize gl3w");
    }

    // Disable v-sync
    SDL_GL_SetSwapInterval(0);

    m_lastUpdate = std::chrono::system_clock::now();
}

void Platform::destroy() {
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Platform::update() {
    // Update frame time
    std::chrono::system_clock::time_point prev = m_lastUpdate;
    m_lastUpdate = std::chrono::system_clock::now();
    m_frameTime = std::chrono::duration_cast<std::chrono::microseconds>(m_lastUpdate - prev).count() / 1000.0f;

    // Change "pressed" states to "down" states
    for (auto& keyState : m_keyStates) {
        if (keyState.second == KEY_STATE_PRESSED) {
            keyState.second = KEY_STATE_DOWN;
        }
    }

    // Poll events from system
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                core->quit();
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        core->state.platformEventFlags |= PLATFORM_EVENT_FRAME_RESIZED;
                        core->state.frameWidth = event.window.data1;
                        core->state.frameHeight = event.window.data2;
                        break;
                }
                break;
            case SDL_KEYDOWN:
                if (m_keyStates[event.key.keysym.sym] == KEY_STATE_UP) {
                    m_keyStates[event.key.keysym.sym] = KEY_STATE_PRESSED;
                }
                break;
            case SDL_KEYUP:
                m_keyStates[event.key.keysym.sym] = KEY_STATE_UP;
                break;
        }
    }
}

void Platform::swapBuffers() {
    SDL_GL_SwapWindow(m_window);
}

bool Platform::isKeyPressed(u32 key) {
    return m_keyStates[key] == KEY_STATE_PRESSED;
}

bool Platform::isKeyDown(u32 key) {
    return m_keyStates[key] != KEY_STATE_UP;
}

f32 Platform::getFrameTime() {
    return m_frameTime;
}
