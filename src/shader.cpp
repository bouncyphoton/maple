#include "shader.h"
#include "core.h"
#include "utils.h"
#include <GL/gl3w.h>
#include <vector>

// TODO: clean up some repetitive code

void Shader::init(const char *vertex_path, const char *fragment_path) {
    u32 vert = 0, frag = 0;
    m_programId = glCreateProgram();
    if (m_programId == 0) {
        core->fatal("Failed to create shader program");
    }

    std::string vertexSrc = utils::load_file_to_string(vertex_path);
    std::string fragmentSrc = utils::load_file_to_string(fragment_path);

    // Create and compile vertex and fragment shader
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
    m_uniformLocations.clear();
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

    // Attach if successfully compiled
    glAttachShader(m_programId, shader);

    return shader;
}

void Shader::setInt(const std::string &name, s32 value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setVec3(const std::string &name, glm::vec3 value) {
    glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

u32 Shader::getUniformLocation(const std::string &name) {
    auto it = m_uniformLocations.find(name);
    if (it == m_uniformLocations.end()) {
        m_uniformLocations[name] = glGetUniformLocation(m_programId, name.c_str());
    }

    return m_uniformLocations[name];
}

void ComputeShader::init(const char *compute_path) {
    u32 shader = 0;
    m_programId = glCreateProgram();
    if (m_programId == 0) {
        core->fatal("Failed to create shader program");
    }

    // Get shader source as string
    std::string shaderSrc = utils::load_file_to_string(compute_path);

    // Create and compile compute shader
    shader = compileAndAttach(GL_COMPUTE_SHADER, shaderSrc.c_str(), compute_path);

    // Link program
    glLinkProgram(m_programId);
    glDetachShader(m_programId, shader);
    glDeleteShader(shader);

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

void ComputeShader::dispatchCompute(u32 num_groups_x, u32 num_groups_y, u32 num_groups_z) {
    glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}
