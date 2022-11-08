#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aModelMat;

out vec2 vTexCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * aModelMat * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
}
