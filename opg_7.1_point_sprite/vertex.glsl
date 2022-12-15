#version 460 core

layout (location = 0) in vec3 aPosition;

uniform mat4 view;
uniform mat4 model;

void main() {

    // in view space (camera space)
    gl_Position = view * model * vec4(aPosition, 1.0);
}
