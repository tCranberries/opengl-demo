#version 460 core

layout (location = 0) in vec3 aPosition;

uniform mat4 lightProjection;
uniform mat4 lightView;
uniform mat4 model;

void main() {
    // Vertex Shader的输出就是在 Clip Space 上, 接着由GPU自己做透视除法将顶点转到 NDC
    // https://zhuanlan.zhihu.com/p/65969162
    gl_Position = lightProjection * lightView * model * vec4(aPosition, 1.0);
}