#version 330 core

// 获取深度贴图的 vs
layout (location = 0) in vec3 aPosition;

uniform mat4 model;

void main() {
    // Vertex Shader的输出就是在 Clip Space 上, 接着由GPU自己做透视除法将顶点转到 NDC
    // https://zhuanlan.zhihu.com/p/65969162
    gl_Position = model * vec4(aPosition, 1.0);
}
