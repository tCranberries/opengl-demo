#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
    vTexCoord = aTexCoord;
    gl_Position = projection * view * vec4(aPosition, 1.0);
}
