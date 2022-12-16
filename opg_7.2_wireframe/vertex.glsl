#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 vNormal;
// view space fragment position
out vec3 vFragPos;
out vec2 vTexCoord;


uniform mat3 normalMatrix;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {

    vFragPos = vec3(view * model * vec4(aPosition, 1.0));

    vNormal = normalMatrix * aNormal;
    vTexCoord = aTexCoord;
    // in view space (camera space)
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
