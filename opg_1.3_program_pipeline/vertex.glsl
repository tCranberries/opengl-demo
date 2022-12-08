#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

// why
out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};


uniform vec3 colorMask;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    // 所有的变量都必须在声明的时候进行初始化
    vColor = aColor * colorMask;
    gl_Position = projection * view *model * vec4(aPosition, 1.0);
}
