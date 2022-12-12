#version 460 core

layout (location = 0) in vec3 aPosition;


//uniform mat4 projection;
//uniform mat4 view;



void main() {
    // 所有的变量都必须在声明的时候进行初始化

    gl_Position = vec4(aPosition, 1.0);
}
