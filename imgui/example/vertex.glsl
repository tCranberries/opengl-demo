#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vColor;
out vec2 vTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main() {
    // 所有的变量都必须在声明的时候进行初始化
    vColor = aColor;
    vTexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
