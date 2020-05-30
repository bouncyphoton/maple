#version 450 core
layout (location = 0) out vec4 oFragColor;

in VertexData {
    vec2 uv;
} i;

uniform sampler2D uTexture;

void main() {
    oFragColor = texture(uTexture, i.uv);
}
