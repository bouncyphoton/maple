#include "renderer.h"
#include "core.h"
#include <GL/gl3w.h>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::init() {
    // Set default state
    glEnable(GL_DEPTH_TEST);

    // Initialize dummy vao
    glGenVertexArrays(1, &m_dummyVao);
    if (m_dummyVao == 0) { core->fatal("Failed to generate dummy vao"); }

    // Initialize output texture
    // TODO: write wrapper for opengl texture
    glGenTextures(1, &m_outTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_outTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, core->state.frameWidth, core->state.frameHeight,
                 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(0, m_outTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    // Set/reset frame number
    m_frameNumber = 0;
    m_partitionIdx = 0;

    // Initialize shaders
    m_fullscreenTextureShader.init("../assets/shaders/fullscreen.vert", "../assets/shaders/fullscreen.frag");
    m_meshShader.init("../assets/shaders/mesh.vert", "../assets/shaders/mesh.frag");
    m_pathTraceShader.init("../assets/shaders/trace.comp");
}

void Renderer::destroy() {
    m_pathTraceShader.destroy();
    m_meshShader.destroy();
    m_fullscreenTextureShader.destroy();
    glDeleteTextures(1, &m_outTexture);
    glDeleteVertexArrays(1, &m_dummyVao);
}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera &camera = core->state.camera;
    f32 aspectRatio = (f32) core->state.frameWidth / (f32) core->state.frameHeight;

    u32 totalPartitions = core->state.partitionsPerSide * core->state.partitionsPerSide;
    // TODO: make sure we get the whole thing
    u32 width = core->state.frameWidth / core->state.partitionsPerSide;
    u32 height = core->state.frameHeight / core->state.partitionsPerSide;
    u32 xOffset = (m_partitionIdx % core->state.partitionsPerSide) * width;
    u32 yOffset = (m_partitionIdx / core->state.partitionsPerSide) * height;
    m_partitionIdx = (m_partitionIdx + 1) % totalPartitions;

    // Path trace
    {
        m_pathTraceShader.bind();

        // Calculate lower left and upper right vectors, adapted from Peter Shirley's "Ray Tracing in One Weekend"
        f32 halfHeight = tan(camera.fovRadians * 0.5f);
        f32 halfWidth = aspectRatio * halfHeight;
        glm::vec3 front = glm::normalize(camera.lookDir);
        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), -front));
        glm::vec3 up = glm::cross(-front, right);

        glm::vec3 lowerLeft = camera.position - halfWidth * right - halfHeight * up + front;
        glm::vec3 upperRight = lowerLeft + halfWidth * 2.0f * right + halfHeight * 2.0f * up;

        // Set shader uniforms
        m_pathTraceShader.setVec2("uResolution", glm::vec2(core->state.frameWidth, core->state.frameHeight));
        m_pathTraceShader.setVec2("uOffset", glm::vec2(xOffset, yOffset));
        m_pathTraceShader.setVec3("uCamera.position", core->state.camera.position);
        m_pathTraceShader.setVec3("uCamera.lowerLeft", lowerLeft);
        m_pathTraceShader.setVec3("uCamera.upperRight", upperRight);
        m_pathTraceShader.setInt("uFrameNumber", m_frameNumber);

        // Set scene data
        // TODO: Don't do this every frame
        core->state.scene.bind();

        // Dispatch compute shader and wait
        m_pathTraceShader.dispatchCompute(width, height, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }

    // Draw texture to screen via fullscreen texture shader
    {
        // Set state
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_ALWAYS);

        // Draw with dummy vertex array
        m_fullscreenTextureShader.bind();
        m_fullscreenTextureShader.setInt("uTexture", 0);
        glBindVertexArray(m_dummyVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Undo state changes
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }

    if (m_partitionIdx == 0) {
        ++m_frameNumber;
    }
}

void Renderer::resize() {
    // Resize viewport
    glViewport(0, 0, core->state.frameWidth, core->state.frameHeight);

    // Resize output texture, it's ok that old pixel data is wiped
    s32 previouslyBoundTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previouslyBoundTexture);
    glBindTexture(GL_TEXTURE_2D, m_outTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, core->state.frameWidth, core->state.frameHeight,
                 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, previouslyBoundTexture);
    m_frameNumber = 0;
    m_partitionIdx = 0;
}
