#version 450 core
layout (location = 0) out vec4 oFragColor;

in VertexData {
    vec3 position;
    vec3 normal;
} i;

uniform vec3 uDiffuseColor;
uniform vec3 uEmissionColor;

void main() {
    vec3 lightCenter = vec3(-0.005, 1.98, -0.03);

    vec3 N = normalize(i.normal);
    vec3 L = normalize(lightCenter - i.position);
    float atten = max(0, dot(N, L));

    oFragColor = vec4(uDiffuseColor * atten + uEmissionColor, 1);
}
