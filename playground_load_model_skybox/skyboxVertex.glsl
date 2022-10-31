#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 vTexCoord;

uniform mat4 projection;
uniform mat4 view;

void main() {
    vec4 tmpPosition = projection * view * vec4(aPosition, 1.0);
    gl_Position = tmpPosition.xyww;
    vTexCoord = aPosition;
}
