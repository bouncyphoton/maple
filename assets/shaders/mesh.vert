#version 450 core
layout (location = 0) in vec3 aPosition;

out VertexData {
    vec3 position;
} o;

uniform mat4 uViewProjectionMatrix;
uniform mat4 uModelMatrix;

void main() {
    o.position = vec3(uModelMatrix * vec4(aPosition, 1));

    gl_Position = uViewProjectionMatrix * vec4(o.position, 1);
}
