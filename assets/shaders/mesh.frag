#version 450 core
layout (location = 0) out vec4 oFragColor;

in VertexData {
    vec3 position;
} i;

uniform vec3 uDiffuseColor;
uniform vec3 uEmissionColor;

void main() {
    oFragColor = vec4(uDiffuseColor + uEmissionColor, 1);
}
