#version 450 core
layout (location = 0) out vec4 oFragColor;

in VertexData {
    vec2 uv;
} i;

void main() {
    oFragColor = vec4(0.8, 0.9, 1.0, 1);
}
