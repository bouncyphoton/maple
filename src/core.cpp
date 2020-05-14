#include "core.h"
#include <cstdlib>
#include <iostream>

static Core core_local;
Core *core = &core_local;

void Core::run() {
    info("Initializing...");

    init();

    info("Initialized successfully");

    while (true) {
        m_platform.update();
        handlePlatformEvents();

        m_renderer.render();
        m_platform.swapBuffers();
    }
}

void Core::quit() {
    info("Quitting normally");
    cleanup();
    exit(0);
}

void Core::info(const std::string &msg) {
    std::cout << "[info] " << msg << std::endl;
}

void Core::warn(const std::string &msg) {
    std::cerr << "[warn] " << msg << std::endl;
}

void Core::fatal(const std::string &msg) {
    std::cerr << "[fatal] " << msg << std::endl;
    cleanup();
    exit(1);
}

void Core::init() {
    m_platform.init();
    m_renderer.init();
}

void Core::cleanup() {
    m_renderer.destroy();
    m_platform.destroy();
}

void Core::handlePlatformEvents() {
    if (state.platformEventFlags & PLATFORM_EVENT_FRAME_RESIZED) {
        state.platformEventFlags ^= PLATFORM_EVENT_FRAME_RESIZED;
        info("Resized to " + std::to_string(state.frameWidth) + "x" + std::to_string(state.frameHeight));
        m_renderer.resize();
    }
}
