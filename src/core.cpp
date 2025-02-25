#include "core.h"
#include <glm/gtc/constants.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>

static Core core_local;
Core *core = &core_local;

void Core::run() {
    info("Initializing...");

    init();

    info("Initialized successfully");

    u32 numFrames = 0;
    f32 time = 0;

    while (true) {
        m_platform.update();
        handlePlatformEvents();

        // Reload renderer
        if (m_platform.isKeyPressed(SDLK_F5)) {
            m_renderer.destroy();
            m_renderer.init();
            info("Reloaded renderer");
        }

        m_renderer.render();
        m_platform.swapBuffers();

        // performance
        f32 frameTime = m_platform.getFrameTime();
        ++numFrames;
        time += frameTime;

        if (time > 1000) {
            std::ostringstream perfSummary;
            u32 numPartitions = state.partitionsPerSide * state.partitionsPerSide;
            perfSummary << "performance summary\n";
            perfSummary << "- avg frametime:  " << (time / numFrames) << "ms\n";
            perfSummary << "- samples/pixel:  " << m_renderer.getFrameNumber() << "\n";
            perfSummary << "- samples/second: " << (1000.0f / (numPartitions * frameTime));
            info(perfSummary.str());
            time = 0;
            numFrames = 0;
        }
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
    state.camera.position = glm::vec3(0, 1, 3.5);
    state.camera.lookDir = glm::vec3(0, 0, -1);
    state.camera.fovRadians = glm::quarter_pi<f32>();
    state.camera.nearPlane = 0.001f;
    state.camera.farPlane = 100.0f;

    m_platform.init();
    m_renderer.init();

    info("Loading assets...");
    state.scene.load("../assets/CornellBox/", "CornellBox-Original.obj");
    info("Done!");
}

void Core::cleanup() {
    state.scene.destroy();
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
