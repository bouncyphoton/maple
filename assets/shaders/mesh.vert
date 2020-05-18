#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out VertexData {
    vec3 position;
    vec3 normal;
} o;

uniform mat4 uViewProjectionMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;

void main() {
    o.position = vec3(uModelMatrix * vec4(aPosition, 1));
    o.normal = vec3(uNormalMatrix * vec4(aNormal, 1));

    gl_Position = uViewProjectionMatrix * vec4(o.position, 1);
}
