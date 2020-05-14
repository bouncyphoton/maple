#include "shader.h"
#include "core.h"
#include "utils.h"
#include <GL/gl3w.h>
#include <vector>

void Shader::init(const char *vertex_path, const char *fragment_path) {
    u32 vert = 0, frag = 0;
    m_programId = glCreateProgram();
    if (m_programId == 0) {
        core->fatal("Failed to create shader program");
    }

    std::string vertexSrc = utils::load_file_to_string(vertex_path);
    std::string fragmentSrc = utils::load_file_to_string(fragment_path);

    // Create and compile vertex shader
    vert = compileAndAttach(GL_VERTEX_SHADER, vertexSrc.c_str(), vertex_path);
    frag = compileAndAttach(GL_FRAGMENT_SHADER, fragmentSrc.c_str(), fragment_path);

    // Link program
    glLinkProgram(m_programId);
    glDetachShader(m_programId, vert);
    glDetachShader(m_programId, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    // Check link status
    s32 success;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        GLint length = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &length);

        std::vector<char> log(length);
        glGetProgramInfoLog(m_programId, length, &length, log.data());

        core->warn("Failed to link program:\n" + std::string(log.data()));
    }
}

void Shader::destroy() {
    glDeleteProgram(m_programId);
}

void Shader::bind() {
    glUseProgram(m_programId);
}

u32 Shader::compileAndAttach(u32 shader_type, const char *shader_src, const char *debug_shader_path) {
    u32 shader = glCreateShader(shader_type);
    if (shader == 0) {
        core->fatal("Failed to create shader \"" + std::string(debug_shader_path) + "\"");
    }
    glShaderSource(shader, 1, &shader_src, nullptr);
    glCompileShader(shader);

    // Check compile status
    s32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::vector<char> log(length);
        glGetShaderInfoLog(shader, length, &length, log.data());

        core->warn("Failed to compile shader \"" + std::string(debug_shader_path) + "\":\n" + std::string(log.data()));
    }

    // Attach if successfully
    glAttachShader(m_programId, shader);

    return shader;
}
