#include "renderer.h"
#include "core.h"
#include <GL/gl3w.h>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::init() {
    // Set default state
    glEnable(GL_DEPTH_TEST);

    // Initialize members
    glGenVertexArrays(1, &m_dummyVao);
    if (m_dummyVao == 0) { core->fatal("Failed to generate dummy vao"); }
    m_skyShader.init("../assets/shaders/fullscreen.vert", "../assets/shaders/sky.frag");
    m_meshShader.init("../assets/shaders/mesh.vert", "../assets/shaders/mesh.frag");
}

void Renderer::destroy() {
    m_meshShader.destroy();
    m_skyShader.destroy();
    glDeleteVertexArrays(1, &m_dummyVao);
}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw sky
    /*{
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
    }*/

    // Draw mesh
    {
        m_meshShader.bind();

        Camera &camera = core->state.camera;
        f32 aspectRatio = (f32) core->state.frameWidth / (f32) core->state.frameHeight;

        // set view and projection matrices
        glm::mat4 view = glm::lookAt(
                camera.position,
                camera.position + camera.lookDir,
                glm::vec3(0, 1, 0)
        );
        glm::mat4 proj = glm::perspective(camera.fovRadians, aspectRatio, camera.nearPlane, camera.farPlane);
        m_meshShader.setMat4("uViewProjectionMatrix", proj * view);

        // TODO: make a better system for getting meshes out of a loaded model
        std::vector<Mesh> &meshes = core->assetManager.getModel("../assets/CornellBox/CornellBox-Original.obj").meshes;

        for (auto &mesh : meshes) {
            glm::mat4 modelMatrix = glm::mat4(1);
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

            m_meshShader.setVec3("uDiffuseColor", mesh.diffuseColor);
            m_meshShader.setVec3("uEmissionColor", mesh.emissionColor);
            m_meshShader.setMat4("uModelMatrix", modelMatrix);
            m_meshShader.setMat4("uNormalMatrix", normalMatrix);

            mesh.draw();
        }
    }
}

void Renderer::resize() {
    glViewport(0, 0, core->state.frameWidth, core->state.frameHeight);
}
