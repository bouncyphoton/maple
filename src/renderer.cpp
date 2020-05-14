#include "renderer.h"
#include "core.h"
#include <GL/gl3w.h>

void Renderer::init() {
    // Set default state
    glEnable(GL_DEPTH_TEST);

    // Initialize members
    glGenVertexArrays(1, &m_dummyVao);
    if (m_dummyVao == 0) { core->fatal("Failed to generate dummy vao"); }
    m_skyShader.init("../assets/shaders/fullscreen.vert", "../assets/shaders/sky.frag");
}

void Renderer::destroy() {
    m_skyShader.destroy();
    glDeleteVertexArrays(1, &m_dummyVao);
}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw sky
    {
        // Set state
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_ALWAYS);

        // Draw with dummy vertex array
        m_skyShader.bind();
        glBindVertexArray(m_dummyVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Undo state changes
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }
}

void Renderer::resize() {
    glViewport(0, 0, core->state.frameWidth, core->state.frameHeight);
}
