#version 460 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec3 aColor;

//uniform mat4 projection;
//uniform mat4 view;
uniform mat4 model;

out vec3 color;

void main() {
    // 所有的变量都必须在声明的时候进行初始化
    color = aColor;
    gl_Position = model * vec4(aPosition, 0.0, 1.0);
}
