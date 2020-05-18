#include "mesh.h"
#include "core.h"
#include <GL/gl3w.h>

void Mesh::init(const std::vector<Vertex> &vertices) {
    m_vertices = vertices;

    s32 previouslyBoundVao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previouslyBoundVao);

    glGenVertexArrays(1, &m_vao);
    if (m_vao == 0) {
        core->fatal("Failed to generate VAO for mesh");
    }
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    if (m_vbo == 0) {
        core->fatal("Failed to generate VBO for mesh");
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, normal));

//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, ???));

    glBindVertexArray(previouslyBoundVao);
}

void Mesh::destroy() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Mesh::draw() {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}
